#ifndef CAF_VARIABLES_H
#define CAF_VARIABLES_H

// Standard Library Includes
#include <iostream>
#include <fstream>
#include <string>
#include <array>

// // ROOT Includes
#include "TTree.h"
#include "TChain.h"
#include "TObject.h"
#include "TVector3.h"



class CAFInfo {

public:
        CAFInfo() { initCaloBranches(); }
        ~CAFInfo() {
            for (int plane = 0; plane < kCaloPlaneCount; ++plane) {
                delete[] trk_calo_points_length[plane];
                delete[] trk_calo_dQdx[plane];
                delete[] trk_calo_dEdx[plane];
                delete[] trk_calo_integral[plane];
                delete[] trk_calo_mult[plane];
                delete[] trk_calo_phi[plane];
                delete[] trk_calo_pitch[plane];
                delete[] trk_calo_rr[plane];
                delete[] trk_calo_t[plane];
                delete[] trk_calo_tpc[plane];
                delete[] trk_calo_width[plane];
                delete[] trk_calo_wire[plane];
                delete[] trk_calo_x[plane];
                delete[] trk_calo_y[plane];
                delete[] trk_calo_z[plane];
            }
        }

	// Slice Stuff

	// ------------------ Slice Header Variables ----------------- //
	
	std::string slc_length_str = "rec.slc..length";
	Int_t slc_length = 1000;


	std::string slc_self_str = "rec.slc.self";
	Int_t *slc_self = new Int_t[slc_length];


	// --------------- RECO STUFF -------------------------------------- //

	std::string slc_reco_npfp_tag = "rec.slc.reco.npfp";
	ULong64_t *slc_npfp = new ULong64_t[slc_length];
	

	std::string slc_reco_tag = "rec.slc.reco.pfp.";

	std::string slc_nu_score_str = "rec.slc.nu_score";
	Float_t *slc_nu_score = new Float_t[slc_length];
	
	// PFP Stuff

	std::string trk_tag = "rec.slc.reco.pfp.trk.";

        
	// ------------------ PFP Header Variables ----------------- //
	
	//std::string pfp_length_str = "rec.reco.pfp..length";
	//Int_t pfp_length = 1000;

	//std::string pfp_length_str = "rec.slc.reco.npfp";
	//ULong64_t pfp_length = 1000;
	
	std::string pfp_length_str = "rec.slc.reco.pfp..totarraysize";
	Int_t pfp_length = 1000;
	

	std::string pfp_slcID_str = "rec.slc.reco.pfp.slcID";
	Int_t *pfp_slcID = new Int_t[pfp_length];
	
	std::string pfp_trackScore_str = "rec.slc.reco.pfp.trackScore";
	Float_t *pfp_trackScore = new Float_t[pfp_length];
	
	std::string pfp_id_str = "rec.slc.reco.pfp.id";
	Int_t *pfp_id = new Int_t[pfp_length]; 
	
	std::string pfp_parent_str = "rec.slc.reco.pfp.parent";
	Int_t *pfp_parent = new Int_t[pfp_length]; 

	std::string pfp_parent_is_primary_str = "rec.slc.reco.pfp.parent_is_primary";
	Char_t *pfp_parent_is_primary = new Char_t[pfp_length]; 

	std::string pfp_t0_str = "rec.slc.reco.pfp.t0";
	Float_t *pfp_t0 = new Float_t[pfp_length]; 

	

	// ---------------- Track Stuff -------------------------------- //

	std::string trk_start_x_str = trk_tag + "start.x";
	Float_t *trk_start_x = new Float_t[pfp_length];

	std::string trk_start_y_str = trk_tag + "start.y";
	Float_t *trk_start_y = new Float_t[pfp_length];
	
	std::string trk_start_z_str = trk_tag + "start.z";
	Float_t *trk_start_z = new Float_t[pfp_length];

	std::string trk_end_x_str = trk_tag + "end.x";
	Float_t *trk_end_x = new Float_t[pfp_length];

	std::string trk_end_y_str = trk_tag + "end.y";
	Float_t *trk_end_y = new Float_t[pfp_length];

	std::string trk_end_z_str = trk_tag + "end.z";
	Float_t *trk_end_z = new Float_t[pfp_length];

	std::string trk_dir_x_str = trk_tag + "dir.x";
	Float_t *trk_dir_x = new Float_t[pfp_length];

	std::string trk_dir_y_str = trk_tag + "dir.y";
	Float_t *trk_dir_y = new Float_t[pfp_length];

	std::string trk_dir_z_str = trk_tag + "dir.z";
	Float_t *trk_dir_z = new Float_t[pfp_length];

	std::string trk_p_muon_str = trk_tag + "rangeP.p_muon";
	Float_t *trk_p_muon = new Float_t[pfp_length];
	
	std::string trk_p_proton_str = trk_tag + "rangeP.p_proton";
	Float_t *trk_p_proton = new Float_t[pfp_length];

	std::string trk_p_pion_str = trk_tag + "rangeP.p_pion";
	Float_t *trk_p_pion = new Float_t[pfp_length];
	
	std::string trk_muonScore_str = trk_tag + "dazzle.muonScore";
	Float_t *trk_muonScore = new Float_t[pfp_length];

	std::string trk_protonScore_str = trk_tag + "dazzle.protonScore";
	Float_t *trk_protonScore = new Float_t[pfp_length];

	std::string trk_pionScore_str = trk_tag + "dazzle.pionScore";
	Float_t *trk_pionScore = new Float_t[pfp_length];

	std::string trk_bestScore_str = trk_tag + "dazzle.bestScore";
	Float_t *trk_bestScore = new Float_t[pfp_length];
        
    std::string trk_chis0_muon_str = trk_tag + "chi2pid.0.chi2_muon";
	Float_t *trk_chis0_muon = new Float_t[pfp_length];

    std::string trk_chis0_proton_str = trk_tag + "chi2pid.0.chi2_proton";
	Float_t *trk_chis0_proton = new Float_t[pfp_length];

    std::string trk_chis0_pion_str = trk_tag + "chi2pid.0.chi2_pion";
	Float_t *trk_chis0_pion = new Float_t[pfp_length];
	
    std::string trk_chis1_muon_str = trk_tag + "chi2pid.1.chi2_muon";
	Float_t *trk_chis1_muon = new Float_t[pfp_length];

    std::string trk_chis1_proton_str = trk_tag + "chi2pid.1.chi2_proton";
	Float_t *trk_chis1_proton = new Float_t[pfp_length];

    std::string trk_chis1_pion_str = trk_tag + "chi2pid.1.chi2_pion";
	Float_t *trk_chis1_pion = new Float_t[pfp_length];

    std::string trk_chis2_muon_str = trk_tag + "chi2pid.2.chi2_muon";
	Float_t *trk_chis2_muon = new Float_t[pfp_length];

    std::string trk_chis2_proton_str = trk_tag + "chi2pid.2.chi2_proton";
	Float_t *trk_chis2_proton = new Float_t[pfp_length];

    std::string trk_chis2_pion_str = trk_tag + "chi2pid.2.chi2_pion";
	Float_t *trk_chis2_pion = new Float_t[pfp_length];


	// ------------- Shower/Track Truth Info ---------------------------------------- //
	
	std::string pfp_truth_G4ID_str = "rec.slc.reco.pfp.shw.truth.p.G4ID";
	Int_t *pfp_truth_G4ID = new Int_t[pfp_length];

	std::string pfp_truth_pdg_str = "rec.slc.reco.pfp.shw.truth.p.pdg";
	Int_t *pfp_truth_pdg = new Int_t[pfp_length];

	std::string pfp_truth_interaction_id_str = "rec.slc.reco.pfp.shw.truth.p.interaction_id";
	Int_t *pfp_truth_interaction_id = new Int_t[pfp_length];

	std::string pfp_truth_genT_str = "rec.slc.reco.pfp.shw.truth.p.genT";
	Float_t *pfp_truth_genT = new Float_t[pfp_length];

	// True PFP Variables for Containement
	std::string pfp_truth_contained_str = "rec.slc.reco.pfp.shw.truth.p.contained";
	Char_t *pfp_truth_contained = new Char_t[pfp_length];

	std::string pfp_truth_cont_tpc_str = "rec.slc.reco.pfp.shw.truth.p.cont_tpc";
	Char_t *pfp_truth_cont_tpc = new Char_t[pfp_length];

	std::string pfp_truth_crosses_tpc_str = "rec.slc.reco.pfp.shw.truth.p.crosses_tpc";
	Char_t *pfp_truth_crosses_tpc = new Char_t[pfp_length];

	// Energy Variables

	std::string pfp_truth_genE_str = "rec.slc.reco.pfp.shw.truth.p.genE";
	Float_t *pfp_truth_genE = new Float_t[pfp_length];

	std::string pfp_truth_genp_x_str = "rec.slc.reco.pfp.shw.truth.p.genp.x";
	Float_t *pfp_truth_genp_x = new Float_t[pfp_length];
	std::string pfp_truth_genp_y_str = "rec.slc.reco.pfp.shw.truth.p.genp.y";
	Float_t *pfp_truth_genp_y = new Float_t[pfp_length];
	std::string pfp_truth_genp_z_str = "rec.slc.reco.pfp.shw.truth.p.genp.z";
	Float_t *pfp_truth_genp_z = new Float_t[pfp_length];

	// The end momentum in the TPC Active volume
	std::string pfp_truth_endp_x_str = "rec.slc.reco.pfp.shw.truth.p.endp.x";
	Float_t *pfp_truth_endp_x = new Float_t[pfp_length];
	std::string pfp_truth_endp_y_str = "rec.slc.reco.pfp.shw.truth.p.endp.y";
	Float_t *pfp_truth_endp_y = new Float_t[pfp_length];
	std::string pfp_truth_endp_z_str = "rec.slc.reco.pfp.shw.truth.p.endp.z";
	Float_t *pfp_truth_endp_z = new Float_t[pfp_length];
	
	// True Position Variables

	// Start position in TPC Active Volume
	std::string pfp_truth_start_x_str = "rec.slc.reco.pfp.shw.truth.p.start.x";
	Float_t *pfp_truth_start_x = new Float_t[pfp_length];
	std::string pfp_truth_start_y_str = "rec.slc.reco.pfp.shw.truth.p.start.y";
	Float_t *pfp_truth_start_y = new Float_t[pfp_length];
	std::string pfp_truth_start_z_str = "rec.slc.reco.pfp.shw.truth.p.start.z";
	Float_t *pfp_truth_start_z = new Float_t[pfp_length];

	// End position in TPC Active Volume
	std::string pfp_truth_end_x_str = "rec.slc.reco.pfp.shw.truth.p.end.x";
	Float_t *pfp_truth_end_x = new Float_t[pfp_length];
	std::string pfp_truth_end_y_str = "rec.slc.reco.pfp.shw.truth.p.end.y";
	Float_t *pfp_truth_end_y = new Float_t[pfp_length];
	std::string pfp_truth_end_z_str = "rec.slc.reco.pfp.shw.truth.p.end.z";
	Float_t *pfp_truth_end_z = new Float_t[pfp_length];

	// Generated Start Position
	std::string pfp_truth_gen_x_str = "rec.slc.reco.pfp.shw.truth.p.gen.x";
	Float_t *pfp_truth_gen_x = new Float_t[pfp_length];
	std::string pfp_truth_gen_y_str = "rec.slc.reco.pfp.shw.truth.p.gen.y";
	Float_t *pfp_truth_gen_y = new Float_t[pfp_length];
	std::string pfp_truth_gen_z_str = "rec.slc.reco.pfp.shw.truth.p.gen.z";
	Float_t *pfp_truth_gen_z = new Float_t[pfp_length];


	// Truth Matching Quality
	std::string shw_truth_hit_completeness_str ="rec.slc.reco.pfp.shw.truth.p.hit_completeness";
	Float_t *shw_truth_hit_completeness = new Float_t[pfp_length];

	std::string shw_truth_hit_purity_str = "rec.slc.reco.pfp.shw.truth.p.hit_purity";
	Float_t *shw_truth_hit_purity = new Float_t[pfp_length];
        

	static const int kCaloPlaneCount = 3;
	static const int kMaxCaloPoints = 10000;

	std::array<std::string, kCaloPlaneCount> trk_calo_points_length_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_points_length;

	std::array<std::string, kCaloPlaneCount> trk_calo_points_totlength_str;
	std::array<Int_t, kCaloPlaneCount> trk_calo_points_totlength;

	std::array<std::string, kCaloPlaneCount> trk_calo_dQdx_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_dQdx;
	std::array<std::string, kCaloPlaneCount> trk_calo_dEdx_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_dEdx;
	std::array<std::string, kCaloPlaneCount> trk_calo_integral_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_integral;
	std::array<std::string, kCaloPlaneCount> trk_calo_mult_str;
	std::array<Short_t*, kCaloPlaneCount> trk_calo_mult;
	std::array<std::string, kCaloPlaneCount> trk_calo_phi_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_phi;
	std::array<std::string, kCaloPlaneCount> trk_calo_pitch_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_pitch;
	std::array<std::string, kCaloPlaneCount> trk_calo_rr_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_rr;
	std::array<std::string, kCaloPlaneCount> trk_calo_t_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_t;
	std::array<std::string, kCaloPlaneCount> trk_calo_tpc_str;
	std::array<Short_t*, kCaloPlaneCount> trk_calo_tpc;
	std::array<std::string, kCaloPlaneCount> trk_calo_width_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_width;
	std::array<std::string, kCaloPlaneCount> trk_calo_wire_str;
	std::array<Short_t*, kCaloPlaneCount> trk_calo_wire;
	std::array<std::string, kCaloPlaneCount> trk_calo_x_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_x;
	std::array<std::string, kCaloPlaneCount> trk_calo_y_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_y;
	std::array<std::string, kCaloPlaneCount> trk_calo_z_str;
	std::array<Float_t*, kCaloPlaneCount> trk_calo_z;

	std::array<std::string, kCaloPlaneCount> trk_calo_idx_str;
	std::array<Int_t*, kCaloPlaneCount> trk_calo_idx;	

	std::string calo_tag = "rec.slc.reco.pfp.trk.calo.";

	void initCaloBranches() {
	  for (int plane = 0; plane < kCaloPlaneCount; ++plane) {
	    const std::string prefix = calo_tag + std::to_string(plane);

	    trk_calo_points_length_str[plane] = prefix + "..length";
	    trk_calo_points_length[plane] = new Float_t[pfp_length];

	    trk_calo_points_totlength_str[plane] = prefix + "..totarraysize";
	    trk_calo_points_totlength[plane] = kMaxCaloPoints;

	    trk_calo_dQdx_str[plane] = prefix + ".points.dqdx";
	    trk_calo_dQdx[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_dEdx_str[plane] = prefix + ".points.dedx";
	    trk_calo_dEdx[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_integral_str[plane] = prefix + ".points.integral";
	    trk_calo_integral[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_mult_str[plane] = prefix + ".points.mult";
	    trk_calo_mult[plane] = new Short_t[kMaxCaloPoints];

	    trk_calo_phi_str[plane] = prefix + ".points.phi";
	    trk_calo_phi[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_pitch_str[plane] = prefix + ".points.pitch";
	    trk_calo_pitch[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_rr_str[plane] = prefix + ".points.rr";
	    trk_calo_rr[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_t_str[plane] = prefix + ".points.t";
	    trk_calo_t[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_tpc_str[plane] = prefix + ".points.tpc";
	    trk_calo_tpc[plane] = new Short_t[kMaxCaloPoints];

	    trk_calo_width_str[plane] = prefix + ".points.width";
	    trk_calo_width[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_wire_str[plane] = prefix + ".points.wire";
	    trk_calo_wire[plane] = new Short_t[kMaxCaloPoints];

	    trk_calo_x_str[plane] = prefix + ".points.x";
	    trk_calo_x[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_y_str[plane] = prefix + ".points.y";
	    trk_calo_y[plane] = new Float_t[kMaxCaloPoints];

	    trk_calo_z_str[plane] = prefix + ".points.z";
	    trk_calo_z[plane] = new Float_t[kMaxCaloPoints];

		trk_calo_idx_str[plane] = prefix + ".points..idx";
		trk_calo_idx[plane] = new Int_t[pfp_length];
	  }
	}

        // Set Both Reco and Truth Addresses
	void setCAFInfoAddressesGrid(TChain *tree) {

	  // Slice Stuff
	  tree->SetBranchAddress(slc_length_str.c_str(), &slc_length);
	  tree->SetBranchAddress(slc_self_str.c_str(), slc_self);
	  tree->SetBranchAddress(slc_reco_npfp_tag.c_str(), slc_npfp);
	  tree->SetBranchAddress(slc_nu_score_str.c_str(), slc_nu_score);
          
	  // PFP Header Info 
	  tree->SetBranchAddress(pfp_length_str.c_str(), &pfp_length);
	  tree->SetBranchAddress(pfp_slcID_str.c_str(), pfp_slcID);
	  tree->SetBranchAddress(pfp_id_str.c_str(), pfp_id);
	  tree->SetBranchAddress(pfp_trackScore_str.c_str(), pfp_trackScore);
	  tree->SetBranchAddress(pfp_parent_str.c_str(), pfp_parent);
	  tree->SetBranchAddress(pfp_parent_is_primary_str.c_str(), pfp_parent_is_primary);
	  tree->SetBranchAddress(pfp_t0_str.c_str(), pfp_t0);

	  // Track Stuff
	  tree->SetBranchAddress(trk_start_x_str.c_str(), trk_start_x);
	  tree->SetBranchAddress(trk_start_y_str.c_str(), trk_start_y);
	  tree->SetBranchAddress(trk_start_z_str.c_str(), trk_start_z);
	  tree->SetBranchAddress(trk_end_x_str.c_str(), trk_end_x);
	  tree->SetBranchAddress(trk_end_y_str.c_str(), trk_end_y);
	  tree->SetBranchAddress(trk_end_z_str.c_str(), trk_end_z);
	  tree->SetBranchAddress(trk_dir_x_str.c_str(), trk_dir_x);
	  tree->SetBranchAddress(trk_dir_y_str.c_str(), trk_dir_y);
	  tree->SetBranchAddress(trk_dir_z_str.c_str(), trk_dir_z);
	  tree->SetBranchAddress(trk_p_muon_str.c_str(), trk_p_muon);
	  tree->SetBranchAddress(trk_p_proton_str.c_str(), trk_p_proton);
	  tree->SetBranchAddress(trk_p_pion_str.c_str(), trk_p_pion);
	  tree->SetBranchAddress(trk_muonScore_str.c_str(), trk_muonScore);
	  tree->SetBranchAddress(trk_protonScore_str.c_str(), trk_protonScore);
	  tree->SetBranchAddress(trk_pionScore_str.c_str(), trk_pionScore);
	  tree->SetBranchAddress(trk_bestScore_str.c_str(), trk_bestScore);
	  tree->SetBranchAddress(trk_chis0_muon_str.c_str(), trk_chis0_muon);
	  tree->SetBranchAddress(trk_chis0_proton_str.c_str(), trk_chis0_proton);
	  tree->SetBranchAddress(trk_chis0_pion_str.c_str(), trk_chis0_pion);
	  tree->SetBranchAddress(trk_chis1_muon_str.c_str(), trk_chis1_muon);
	  tree->SetBranchAddress(trk_chis1_proton_str.c_str(), trk_chis1_proton);
	  tree->SetBranchAddress(trk_chis1_pion_str.c_str(), trk_chis1_pion);
	  tree->SetBranchAddress(trk_chis2_muon_str.c_str(), trk_chis2_muon);
	  tree->SetBranchAddress(trk_chis2_proton_str.c_str(), trk_chis2_proton);
	  tree->SetBranchAddress(trk_chis2_pion_str.c_str(), trk_chis2_pion);

	  for (int plane = 0; plane < kCaloPlaneCount; ++plane) {
	    tree->SetBranchAddress(trk_calo_points_length_str[plane].c_str(), trk_calo_points_length[plane]);
	    tree->SetBranchAddress(trk_calo_points_totlength_str[plane].c_str(), &trk_calo_points_totlength[plane]);
	    tree->SetBranchAddress(trk_calo_dQdx_str[plane].c_str(), trk_calo_dQdx[plane]);
	    tree->SetBranchAddress(trk_calo_dEdx_str[plane].c_str(), trk_calo_dEdx[plane]);
	    tree->SetBranchAddress(trk_calo_integral_str[plane].c_str(), trk_calo_integral[plane]);
	    tree->SetBranchAddress(trk_calo_mult_str[plane].c_str(), trk_calo_mult[plane]);
	    tree->SetBranchAddress(trk_calo_phi_str[plane].c_str(), trk_calo_phi[plane]);
	    tree->SetBranchAddress(trk_calo_pitch_str[plane].c_str(), trk_calo_pitch[plane]);
	    tree->SetBranchAddress(trk_calo_rr_str[plane].c_str(), trk_calo_rr[plane]);
	    tree->SetBranchAddress(trk_calo_t_str[plane].c_str(), trk_calo_t[plane]);
	    tree->SetBranchAddress(trk_calo_tpc_str[plane].c_str(), trk_calo_tpc[plane]);
	    tree->SetBranchAddress(trk_calo_width_str[plane].c_str(), trk_calo_width[plane]);
	    tree->SetBranchAddress(trk_calo_wire_str[plane].c_str(), trk_calo_wire[plane]);
	    tree->SetBranchAddress(trk_calo_x_str[plane].c_str(), trk_calo_x[plane]);
	    tree->SetBranchAddress(trk_calo_y_str[plane].c_str(), trk_calo_y[plane]);
	    tree->SetBranchAddress(trk_calo_z_str[plane].c_str(), trk_calo_z[plane]);
		tree->SetBranchAddress(trk_calo_idx_str[plane].c_str(), trk_calo_idx[plane]);
	  }

	  // CRT Stuff
	  //tree->SetBranchAddress(trk_crtspacepoint_x_str.c_str(), trk_crtspacepoint_x);
	  //tree->SetBranchAddress(trk_crtspacepoint_y_str.c_str(), trk_crtspacepoint_y);
	  //tree->SetBranchAddress(trk_crtspacepoint_z_str.c_str(), trk_crtspacepoint_z);

	  // Truth Identification
	  tree->SetBranchAddress(pfp_truth_G4ID_str.c_str(), pfp_truth_G4ID);
	  tree->SetBranchAddress(pfp_truth_pdg_str.c_str(), pfp_truth_pdg);
	  tree->SetBranchAddress(pfp_truth_interaction_id_str.c_str(), pfp_truth_interaction_id);
	  tree->SetBranchAddress(pfp_truth_genT_str.c_str(), pfp_truth_genT);

	  // Containment Type variables
	  tree->SetBranchAddress(pfp_truth_contained_str.c_str(), pfp_truth_contained);
	  tree->SetBranchAddress(pfp_truth_cont_tpc_str.c_str(), pfp_truth_cont_tpc);
	  tree->SetBranchAddress(pfp_truth_crosses_tpc_str.c_str(), pfp_truth_crosses_tpc);

	  // Energy Type Variables
	  tree->SetBranchAddress(pfp_truth_genE_str.c_str(), pfp_truth_genE);

	  tree->SetBranchAddress(pfp_truth_genp_x_str.c_str(), pfp_truth_genp_x);
	  tree->SetBranchAddress(pfp_truth_genp_y_str.c_str(), pfp_truth_genp_y);
	  tree->SetBranchAddress(pfp_truth_genp_z_str.c_str(), pfp_truth_genp_z);

	  tree->SetBranchAddress(pfp_truth_endp_x_str.c_str(), pfp_truth_endp_x);
	  tree->SetBranchAddress(pfp_truth_endp_y_str.c_str(), pfp_truth_endp_y);
	  tree->SetBranchAddress(pfp_truth_endp_z_str.c_str(), pfp_truth_endp_z);

	  // Position Type variables
	  tree->SetBranchAddress(pfp_truth_start_x_str.c_str(), pfp_truth_start_x);
	  tree->SetBranchAddress(pfp_truth_start_y_str.c_str(), pfp_truth_start_y);
	  tree->SetBranchAddress(pfp_truth_start_z_str.c_str(), pfp_truth_start_z);

	  tree->SetBranchAddress(pfp_truth_end_x_str.c_str(), pfp_truth_end_x);
	  tree->SetBranchAddress(pfp_truth_end_y_str.c_str(), pfp_truth_end_y);
	  tree->SetBranchAddress(pfp_truth_end_z_str.c_str(), pfp_truth_end_z);

	  tree->SetBranchAddress(pfp_truth_gen_x_str.c_str(), pfp_truth_gen_x);
	  tree->SetBranchAddress(pfp_truth_gen_y_str.c_str(), pfp_truth_gen_y);
	  tree->SetBranchAddress(pfp_truth_gen_z_str.c_str(), pfp_truth_gen_z);

    } 


private:
// might not need this


};


#endif



