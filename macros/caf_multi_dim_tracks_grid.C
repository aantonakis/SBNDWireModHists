// module to analyze track calorimetry information from CAFs in the context of WireMod

#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TFile.h"
#include "THnSparse.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TVector3.h"
#include "TPRegexp.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "Math/Vector3D.h"


// Calibration Headers from Sungbin
#include "mylib.h"
#include "BetheBloch.h"
#include "SCECorr.h"
#include "YZCorr.h"

// Custom Helper Code
//#include "../../include/CalibrationStandard.h"
#include "CalibrationStandard.h"
#include "CAFVariables.h"
#include "Angles.h"

#include "SelectionWire.h"


using ROOT::Math::XYZVector;


std::vector<TString> filenames_from_input(const TString&, int);
TString basename_prefix(const TString&, const TString& prefix="", const TString& suffix="");
bool is_int(Float_t);
bool is_one_third(float x, float tol = 1e-4);
bool is_two_thirds(float x, float tol = 1e-4);

const UInt_t kNdims = 10;

const UInt_t kNplanes = 3;
const UInt_t kNTPCs = 2;
//const Float_t kTrackCut = 60.; // cm

const TString kTitles[kNdims] = { "x", "y", "z", "txz", "txy", "dqdx", "Q", "W", "G", "P"};


BetheBloch *muon_BB = new BetheBloch(13); // setup for muons
SCECorr *sce_corr_mc = new SCECorr(false);
SCECorr *sce_corr_data = new SCECorr(true);
YZCorr *yz_corr = new YZCorr();
double lifetime = 35.; // mc default --> Changed for gen2

void caf_multi_dim_tracks_grid(TString list_file, TString out_suffix, TString config

) {


    // Read the analysis config

    TEnv env(config);

    bool apply_sce = env.GetValue("apply_sce", false);
    bool apply_yz = env.GetValue("apply_yz", false);
    bool apply_elife = env.GetValue("apply_elife", false);
    bool apply_recom = env.GetValue("apply_recom", false);
    bool isData = env.GetValue("isData", false);
   

    bool tpc_sel = env.GetValue("tpc_sel", false);
    bool crt_sel = env.GetValue("crt_sel", false);
    bool pathological_sel = env.GetValue("pathological_sel", false);
    bool life_sel = env.GetValue("life_sel", false);
    

    std::istringstream issA(env.GetValue("Nbins", ""));
    std::vector<Int_t> Nbins;
    Int_t vA;
    while (issA >> vA) Nbins.push_back(vA);
    
    std::istringstream issB(env.GetValue("Xmin", ""));
    std::vector<Double_t> Xmin;
    Double_t vB;
    while (issB >> vB) Xmin.push_back(vB);

    std::istringstream issC(env.GetValue("Xmax", ""));
    std::vector<Double_t> Xmax;
    Double_t vC;
    while (issC >> vC) Xmax.push_back(vC);

    std::istringstream issD(env.GetValue("dim", ""));
    std::vector<Int_t> dim;
    Int_t vD;
    while (issD >> vD) dim.push_back(vD);


    Float_t kTrackCut = env.GetValue("trkLCut", 60.0);
    Float_t kTXWHighCut = env.GetValue("TXWHighCut", 90.0);
    Int_t kMultCut = env.GetValue("MultCut", 1);
    Float_t kGoodnessCut = env.GetValue("GoodnessCut", 100.0);



    // CAF variable cut values
    Float_t kNeutrinoScoreCut = env.GetValue("NeutrinoScoreCut", 0.1);
    Float_t kTrackScoreCut = env.GetValue("TrackScoreCut", 0.5);
    Float_t kChisMuonCut = env.GetValue("ChisMuonCut", 40.);
    Float_t kChisProtonCut = env.GetValue("ChisProtonCut", 80.);
    bool kProtonSel = env.GetValue("ProtonSel", true);


    std::cout << std::endl;    
    std::cout << "/---------------------------------------------------------------------------/" << std::endl;
    std::cout << std::endl;    
    std::cout << "Script Config:" << std::endl;
    std::cout << "Is this Data? " << isData << std::endl;
    std::cout << "Apply SCE: " << apply_sce << std::endl;
    std::cout << "Apply YZ: " << apply_yz << std::endl;
    std::cout << "Apply Lifetime: " << apply_elife << std::endl;
    std::cout << "Apply Calibration Const./Recomb.: " << apply_recom << std::endl;
    std::cout << std::endl;    
    std::cout << "Selections: " << std::endl;
    std::cout << "TPC Selection: " << tpc_sel << std::endl;
    std::cout << "CRT Selection: " << crt_sel << std::endl;
    std::cout << "Pathological Hit Selection: " << pathological_sel << std::endl;
    std::cout << "Lifetime Calibration Selection: " << life_sel << std::endl;
    std::cout << "DEBUG: kNplanes " << kNplanes << std::endl;
    std::cout << std::endl; 
    std::cout << "Extra Cuts:" << std::endl;
    std::cout << "TrackCut: " << kTrackCut << " cm" << std::endl;
    std::cout << "TXWHighCut: " << kTXWHighCut << " degrees" << std::endl;
    std::cout << "MultCut: > " << kMultCut << " hits are cut" << std::endl; 
    std::cout << "GoodnessCut: > " << kGoodnessCut << " hits are cut" << std::endl; 
    std::cout << "Number of dimensions to keep: " << dim.size() << std::endl;
    std::cout << std::endl; 
   
    std::cout << "/---------------------------------------------------------------------------/" << std::endl;
    std::cout << std::endl;    



    // File List Management
    TChain *fChain = new TChain("recTree");
    TString input_file_dir = getenv("DATA_PATH");
    TString sample_list_dir = getenv("SAMPLE_PATH");
    TString sample_list_label = getenv("FILELIST_LABEL");
    
    TString fileListPath = sample_list_dir + "/" + list_file;
    cout << "Opening : " << fileListPath << endl;

    std::ifstream file(fileListPath.Data());  // Convert TString to const char*
    if (!file) {
      cout << "File does not exist: " << fileListPath << endl;
      cout << "Exiting [caf_multi_dim_tracks_grid]" << endl;
      return;
    }

    AddFilesToChain(fileListPath, fChain);
    //MyCalib my(fChain);

    // SCE Calibration Initialization
    if (apply_sce) {
      if (isData) {
	sce_corr_data -> ReadHistograms();
        // TODO DEBUG
        std::cout << "DATA DEBUG: Read SCE Hists" << std::endl;
      }
      else {
	sce_corr_mc -> ReadHistograms();
      }
    }
   
    // YZ Calibration Initialization
    if (apply_yz) {
      initialize_yz_fallprod(yz_corr, isData);
      yz_corr -> ReadHistograms();
      std::cout << "DATA DEBUG: Initialized YZ" << std::endl;
    }

    TH1::AddDirectory(0);
 
    // 1 hist per plane per TPC. We also keep track of the number of tracks in
    // each eventual projection bin using TH2Is
    THnSparseD* h[kNplanes * kNTPCs];
    THnSparseD* hTracks[kNplanes * kNTPCs];
    THnSparseD* hTrackFlags[kNplanes * kNTPCs];

    for (unsigned i = 0; i < kNplanes * kNTPCs; i++) {
      //THnSparseD* h_temp = new THnSparseD(Form("h%d", i), "", kNdims, kNbins, kXmin, kXmax);
      //THnSparseD* h_temp_trk = new THnSparseD(Form("hTrack%d", i), "", kNdims, kNbins, kXmin, kXmax);
      //THnSparseD* h_temp_trk_flag = new THnSparseD(Form("hTrackFlags%d", i), "", kNdims, kNbins, kXmin, kXmax);
      
      THnSparseD* h_temp = new THnSparseD(Form("h%d", i), "", kNdims, Nbins.data(), Xmin.data(), Xmax.data());
      THnSparseD* h_temp_trk = new THnSparseD(Form("hTrack%d", i), "", kNdims, Nbins.data(), Xmin.data(), Xmax.data());
      THnSparseD* h_temp_trk_flag = new THnSparseD(Form("hTrackFlags%d", i), "", kNdims, Nbins.data(), Xmin.data(), Xmax.data());

      //h[i] = new THnSparseD(Form("h1D%d", i), "", kNdimsP, kNbinsP, kXminP, kXmaxP);
      h[i] = static_cast<THnSparseD*>( h_temp->Projection(dim.size(), dim.data()) );
      hTracks[i] = static_cast<THnSparseD*>( h_temp_trk->Projection(dim.size(), dim.data()) );
      hTrackFlags[i] = static_cast<THnSparseD*>( h_temp_trk_flag->Projection(dim.size(), dim.data()) );
      h[i]->SetName(Form("hHit%d", i));
      hTracks[i]->SetName(Form("hTrack%d", i));

      // Set The axes labels       
      for (int j = 0; j < dim.size(); ++j) {
        h[i]->GetAxis(j)->SetTitle(kTitles[dim[j]]);
        hTracks[i]->GetAxis(j)->SetTitle(kTitles[dim[j]]);
      }

      h_temp->Delete();
      h_temp_trk->Delete();
      h_temp_trk_flag->Delete();
    }

    // Time to loop over PFPs in CAF file and determine and analyze their track calorimetry information

    Long64_t n_entries = fChain->GetEntries();
    std::cout << "This tree has " << n_entries << " events ..." << std::endl;

    // Header Variables
    UInt_t run;
    UInt_t subrun;
    UInt_t evt;
    UInt_t ngenevt;
    //float pot = 0.;
    fChain->SetBranchAddress("rec.hdr.run", &run);
    fChain->SetBranchAddress("rec.hdr.subrun", &subrun);
    fChain->SetBranchAddress("rec.hdr.evt", &evt);
    fChain->SetBranchAddress("rec.hdr.ngenevt", &ngenevt);

    CAFInfo pfp_info;
    pfp_info.setCAFInfoAddressesGrid(fChain);

    std::cout << "Start Event Loop ..." << std::endl;
    // Start loop over events which are slices in the flat cafs effectively
    int count = 0;
    for (Long64_t i = 0; i < n_entries; ++i) {
        fChain->GetEntry(i);

      	
        //if (count % 10 == 0) {
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "// ------------------ Processing event ------------------ // " << count << std::endl;
        std::cout << "Run " << run << " subrun " << subrun << std::endl;
        std::cout << std::endl;

	    std::cout << "Loop over pfps ..." << std::endl;
	    // Loop over the reco pfps --> Should be same number of showers and tracks !!!!
	    std::cout << "Number of pfps " << pfp_info.pfp_length << std::endl;
	    for (int pfp = 0; pfp < pfp_info.pfp_length; ++pfp) {

            // Main cuts are the Track Score and cosnuvtx --> Fits take care of the rest 

            if (pfp_info.pfp_trackScore[pfp] <= kTrackScoreCut) continue;

            int curr_slc = pfp_info.pfp_slcID[pfp];
            float nu_score = -1.;
            for (int s = 0; s < pfp_info.slc_length; ++s) {
                if (pfp_info.slc_self[s] == curr_slc) {
                nu_score = pfp_info.slc_nu_score[s];
                }     
            }
 
            if (nu_score < kNeutrinoScoreCut) continue;
            
            std::vector<float> chi_muon_vec{pfp_info.trk_chis0_muon[pfp], pfp_info.trk_chis1_muon[pfp], pfp_info.trk_chis2_muon[pfp]};
            std::vector<float> chi_proton_vec{pfp_info.trk_chis0_proton[pfp], pfp_info.trk_chis1_proton[pfp], pfp_info.trk_chis2_proton[pfp]};
            std::vector<float> chi_pion_vec{pfp_info.trk_chis0_pion[pfp], pfp_info.trk_chis1_pion[pfp], pfp_info.trk_chis2_pion[pfp]};
            float chi_muon_avg = 0.;
            float chi_proton_avg = 0.;
            float chi_pion_avg = 0.;
            float Nmu = 0.;
            float Nproton = 0.;
            float Npion = 0.;

            for (int chiv = 0; chiv < chi_muon_vec.size(); ++chiv) {
                if (chi_muon_vec.at(chiv) > 0) chi_muon_avg += chi_muon_vec.at(chiv); Nmu += 1.; 
                if (chi_proton_vec.at(chiv) > 0) chi_proton_avg += chi_proton_vec.at(chiv); Nproton += 1.; 
                if (chi_pion_vec.at(chiv) > 0) chi_pion_avg += chi_pion_vec.at(chiv); Npion += 1.; 
            }

            if (Nmu > 0) chi_muon_avg /= Nmu;
            if (Nproton > 0) chi_proton_avg /= Nproton;
            if (Npion > 0) chi_pion_avg /= Npion;

            // make a chis2 PID selection for protons or muons
            
            if (kProtonSel && (chi_muon_avg < kChisMuonCut || chi_proton_avg > kChisProtonCut)) continue;
            if (!kProtonSel && (chi_muon_avg > kChisMuonCut || chi_proton_avg < kChisProtonCut)) continue;

            if (kProtonSel)  std::cout << "Found a proton candidate! chis2_proton: " << chi_proton_avg << " chis_muon: " << chi_muon_avg << std::endl;
            if (!kProtonSel) std::cout << "Found a muon candidate! chis2_proton: " << chi_proton_avg << " chis_muon: " << chi_muon_avg << std::endl;

            //if (chi_muon_avg < 30 || chi_proton_avg > 100) continue;

            // Reset N-dimensional Track Counter
            for (unsigned i = 0; i < kNplanes * kNTPCs; i++) {
                hTrackFlags[i]->Reset();
            }
            
            ROOT::Math::XYZVector trk_dir(pfp_info.trk_dir_x[pfp], pfp_info.trk_dir_y[pfp], pfp_info.trk_dir_z[pfp]);

            // Calorimetry loops
            for (int plane = 0; plane < kNplanes; ++plane) {

                float trk_thxz = -180.;
	            float trk_thyz = -180.;

                std::cout << "Looping over calo points for plane " << plane << ", Npoints: " << pfp_info.trk_calo_points_length[plane][pfp] << std::endl;

                //for (int pr = slc_truth.slc_prim_idx[s]; pr < slc_truth.slc_prim_idx[s] + slc_truth.slc_prim_length[s]; ++pr) {
                for (int icalo = pfp_info.trk_calo_idx[plane][pfp]; icalo < pfp_info.trk_calo_idx[plane][pfp] + pfp_info.trk_calo_points_length[plane][pfp]; ++icalo) {
                    
                    // This may kill the "pathological" hits
                    if (pfp_info.trk_calo_mult[plane][icalo] > kMultCut) continue;

                    // hit trains have widths in increments of exactly 0.5
                    // skip hits from these
                    if (is_int(pfp_info.trk_calo_width[plane][icalo] * 2)) continue;

                    // Angle code goes here!
	                get_dir(trk_thxz, trk_thyz, pfp_info.trk_calo_tpc[plane][icalo], plane, 
                        pfp_info.trk_dir_x[pfp], pfp_info.trk_dir_y[pfp], pfp_info.trk_dir_z[pfp]);

                    // Cut out large tracks 
                    if (std::abs(trk_thxz) > kTXWHighCut) continue;
     
                    // Remove Leakage Hits
                    if ( (pfp_info.trk_calo_x[plane][icalo] < 0) && (pfp_info.trk_calo_tpc[plane][icalo] == 1) ) continue;
                    if ( (pfp_info.trk_calo_x[plane][icalo] > 0) && (pfp_info.trk_calo_tpc[plane][icalo] == 0) ) continue;

                    XYZVector sp(pfp_info.trk_calo_x[plane][icalo], pfp_info.trk_calo_y[plane][icalo], pfp_info.trk_calo_z[plane][icalo]);
                    
                    // Add Pathological Hit Selection
                    Double_t PATHOLOGICAL = 0.5;

                    unsigned IDX = plane + kNplanes * pfp_info.trk_calo_tpc[plane][icalo];
                    bool cut_pathological = false;
	            try {
	                    cut_pathological = txz_cut(trk_thxz, pfp_info.trk_calo_width[plane][icalo], IDX, isData); 
                    }
                    catch (const std::exception &e) {
                        // Code to handle the error
                        std::cerr << "Cut Pathological Error: " << e.what() << std::endl;
                    }

	            if (cut_pathological) PATHOLOGICAL = 1.5;
	
	                // Can remove pathological hits if needed
                    if ( (pathological_sel) && (cut_pathological) ) continue;	

                    // ****** Add Calibration Block here if necessary!!!!! ****** //

                    // Histogram Filling Block
                    std::vector<double> vals;
                    vals.reserve(dim.size());
                    for (int v = 0; v < dim.size(); ++v) {
	                    double dim_val = 0;
	                    if (dim[v] == 0) dim_val = sp.X();
	                    if (dim[v] == 1) dim_val = sp.Y();
	                    if (dim[v] == 2) dim_val = sp.Z();
	                    if (dim[v] == 3) dim_val = trk_thxz;
	                    if (dim[v] == 4) dim_val = trk_thyz;
	                    if (dim[v] == 5) dim_val = pfp_info.trk_calo_dQdx[plane][icalo];
	                    if (dim[v] == 6) dim_val = pfp_info.trk_calo_integral[plane][icalo];
	                    if (dim[v] == 7) dim_val = pfp_info.trk_calo_width[plane][icalo];
	                    if (dim[v] == 8) dim_val = 1; // I don't have goodness for CAFs
	                    if (dim[v] == 9) dim_val = PATHOLOGICAL;
                        vals.push_back(dim_val);
	            }
                    // select by TPC
                    unsigned hit_idx = plane + kNplanes * pfp_info.trk_calo_tpc[plane][icalo];

                    // Fill the results
                    h[hit_idx]->Fill(vals.data());
                    if (hTrackFlags[hit_idx]->GetBinContent(hTrackFlags[hit_idx]->GetBin(vals.data())) == 0) {
                        hTrackFlags[hit_idx]->Fill(vals.data());
                        hTracks[hit_idx]->Fill(vals.data());
                    }

                } // calo points loop

            } // calo plane loop
                       
        } // pfp loop

    } // event loop

    //delete hTrackFlag;
    for (unsigned i = 0; i < kNplanes * kNTPCs; i++) {
      delete hTrackFlags[i];
    }
    std::cout << "Finished the event loop ..." << std::endl;       
    
    std::cout << "About to write histograms to the output file" << std::endl;

    TString output_rootfile_dir = getenv("OUTPUTROOT_PATH");
    TString output_file_name = output_rootfile_dir + "/output_multi_dim_tracks_" + out_suffix + ".root";
    out_rootfile = new TFile(output_file_name, "RECREATE");
    out_rootfile -> cd();
    for (unsigned i = 0; i < kNplanes * kNTPCs; i++) {
	std::cout << "Writing histograms for plane " << i << std::endl;
        h[i]->Write();
        hTracks[i]->Write();
    }
   
    out_rootfile->Close();
    std::cout << "Complete!" << std::endl;

}

std::vector<TString> filenames_from_input(const TString& input_arg, int nmax=-1) {
    std::vector<TString> filenames;
    if (input_arg.EndsWith(".root")) {
        filenames.push_back(input_arg);
        return filenames;
    }

    // read from file
    size_t nfiles = 0;
    std::ifstream ifile(input_arg);
    std::string line;
    while (std::getline(ifile, line)) {
        nfiles++;
        fprintf(stdout, "Adding file %zu: %s...\n", nfiles, line.c_str());
        filenames.push_back(TString(line));
        if (nfiles >= nmax && nmax > 0) break;
    }
    return filenames;
}


TString basename_prefix(const TString& input, const TString& prefix, const TString& suffix) {
    // remove path from filename and return new string with prefix or suffix added before extension
    TPRegexp re(".*/(.*)");
    TObjArray* matches = re.MatchS(input);
    TString result((static_cast<TObjString*>(matches->At(1)))->String());
    matches->Delete();
    return prefix + result;
}


bool is_int(Float_t val) {
    return std::abs(roundf(val) - val) < 0.00001f;
}

bool is_one_third(float x, float tol = 1e-4) {
    return std::fabs(x - 1.0f/3.0f) < tol;
}

bool is_two_thirds(float x, float tol = 1e-4) {
    return std::fabs(x - 2.0f/3.0f) < tol;
}
