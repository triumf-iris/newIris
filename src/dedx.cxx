#include "dedx.h"
#include <iostream>
#include "TString.h"
#ifdef USE_CATIMA
#include "catima/catima.h"
#endif

dedx::dedx()
{
	Clear();	
}

void dedx::Clear()
{
	tables.clear();
}

void dedx::loadIncomingELoss(std::string path, std::string name, std::string foil, std::string tgt, double mass)
{
	printf("Loading %s/lise_%s_in_%s.txt\n",path.data(),name.data(),foil.data());	
	loadELoss(TString::Format("%s/lise_%s_in_%s.txt",path.data(),name.data(),foil.data()).Data(),IrisMaterial::Foil,mass/1000.);	
	printf("Loading %s/lise_%s_in_C4H10.txt\n",path.data(),name.data());	
	loadELoss(TString::Format("%s/lise_%s_in_C4H10.txt",path.data(),name.data()).Data(),IrisMaterial::C4H10,mass/1000.);	
	printf("Loading %s/lise_%s_in_%s.txt\n",path.data(),name.data(),tgt.data());	
	loadELoss(TString::Format("%s/lise_%s_in_%s.txt",path.data(),name.data(),tgt.data()).Data(),IrisMaterial::Target,mass/1000.);	
	printf("Loading %s/lise_%s_in_Si3N4.txt\n\n",path.data(),name.data());	
	loadELoss(TString::Format("%s/lise_%s_in_Si3N4.txt",path.data(),name.data()).Data(),IrisMaterial::Si3N4,mass/1000.);	
}

void dedx::loadOutgoingELoss(std::string path, std::string name, std::string foil, std::string tgt, double mass)
{
	printf("Loading %s/lise_%s_in_%s.txt\n",path.data(),name.data(),foil.data());	
	loadELoss(TString::Format("%s/lise_%s_in_%s.txt",path.data(),name.data(),foil.data()).Data(),IrisMaterial::Foil,mass/1000.);	
	printf("Loading %s/lise_%s_in_Al.txt\n",path.data(),name.data());
	loadELoss(TString::Format("%s/lise_%s_in_Al.txt",path.data(),name.data()).Data(),IrisMaterial::Al,mass/1000.);
	printf("Loading %s/lise_%s_in_B.txt\n",path.data(),name.data());
	loadELoss(TString::Format("%s/lise_%s_in_B.txt",path.data(),name.data()).Data(),IrisMaterial::B,mass/1000.);
	printf("Loading %s/lise_%s_in_CsI.txt\n",path.data(),name.data());	
	loadELoss(TString::Format("%s/lise_%s_in_CsI.txt",path.data(),name.data()).Data(),IrisMaterial::CsI,mass/1000.);	
	printf("Loading %s/lise_%s_in_%s.txt\n",path.data(),name.data(),tgt.data());
	loadELoss(TString::Format("%s/lise_%s_in_%s.txt",path.data(),name.data(),tgt.data()).Data(),IrisMaterial::Target,mass/1000.);
	printf("Loading %s/lise_%s_in_My.txt\n",path.data(),name.data());	
	loadELoss(TString::Format("%s/lise_%s_in_My.txt",path.data(),name.data()).Data(),IrisMaterial::Mylar,mass/1000.);	
	printf("Loading %s/lise_%s_in_P.txt\n",path.data(),name.data());
	loadELoss(TString::Format("%s/lise_%s_in_P.txt",path.data(),name.data()).Data(),IrisMaterial::P,mass/1000.);
	printf("Loading %s/lise_%s_in_Si.txt\n",path.data(),name.data());	
	loadELoss(TString::Format("%s/lise_%s_in_Si.txt",path.data(),name.data()).Data(),IrisMaterial::Si,mass/1000.);	
	printf("Loading %s/lise_%s_in_SiO2.txt\n\n",path.data(),name.data());
	loadELoss(TString::Format("%s/lise_%s_in_SiO2.txt",path.data(),name.data()).Data(),IrisMaterial::SiO2,mass/1000.);
}

void dedx::loadELoss(std::string filename, IrisMaterial material, double m)
{
	std::array<double, 100> e;
	std::array<double, 100> dedx;
	std::ifstream infile;
	char line[2000];
			
	infile.open(filename.data()); 
	if(!infile.is_open()){
		printf("Cannot open the file %s!!\n",filename.data());
		exit(0);
	}

  	infile.getline(line,2000);
	for(int i=0; i<100; i++){
    	infile.getline(line,2000);
    	sscanf(line,"%*f\t%*f\t%lf\t%lf\t%*f\t%*f\t%*f\t%*f\t%*f\t%*f\t%*f\t%*f\t\n",&e[i],&dedx[i]);
  		e[i] *= m/0.931494061; //stopping power for this mass
   	}    
	infile.close();
	tables.emplace(material, make_pair(e, dedx));
}

void dedx::makeCatimaTables(int projectileA, int projectileZ, std::string foil, std::string target, double mass)
{
	#ifdef USE_CATIMA
	catima::Projectile projectile(projectileA, projectileZ);

	catima::Material foilMat;
	if(foil == "Ag") {
		foilMat.add_element(107, 47, 1);
	}
	else {
		std::cout << "Warning: Foil Material not found in dedx.cxx!" << std::endl << "Defaulting to Ag." << std::endl;
		foilMat.add_element(107, 47, 1);
	}

	catima::Material targetMat;
	if(target == "H") {
		targetMat.add_element(1, 1, 1);
	}
	else if(target == "D") {
		targetMat.add_element(2, 1, 1);
	}
	else {
		std::cout << "Warning: Target Material " << target << " not found in dedx.cxx!" << std::endl << "Defaulting to H." << std::endl;
		targetMat.add_element(1, 1, 1);
	}


	catima::Material al;
	catima::Material boron;
	catima::Material c4h10;
	catima::Material csi;
	catima::Material mylar;
	catima::Material phosphorus;
	catima::Material si;
	catima::Material si3n4;
	catima::Material sio2;

	al.add_element(27, 13, 1);
	boron.add_element(11, 5, 1);
	c4h10 = catima::get_material(212);
	csi = catima::get_material(261);
	mylar = catima::get_material(214);
	phosphorus.add_element(31, 15, 1);
	si.add_element(28, 14, 1);
	si3n4.add_element(28, 14, 3);
	si3n4.add_element(14, 7, 4);
	sio2 = catima::get_material(330);

	std::map<IrisMaterial, catima::Material> materialMap;
	materialMap.emplace(IrisMaterial::Foil, foilMat);
	materialMap.emplace(IrisMaterial::Target, targetMat);
	materialMap.emplace(IrisMaterial::Al, al);
	materialMap.emplace(IrisMaterial::B, boron);
	materialMap.emplace(IrisMaterial::C4H10, c4h10);
	materialMap.emplace(IrisMaterial::CsI, csi);
	materialMap.emplace(IrisMaterial::Mylar, mylar);
	materialMap.emplace(IrisMaterial::P, phosphorus);
	materialMap.emplace(IrisMaterial::Si, si);
	materialMap.emplace(IrisMaterial::Si3N4, si3n4);
	materialMap.emplace(IrisMaterial::SiO2, sio2);

	std::array<double, 100> e;
	std::array<double, 100> dedx;

	for(const auto& [irisMat, catMat] : materialMap) {
	
		for(int i = 0; i < 100; i++) {
			auto result = catima::calculate(projectile(energy[i]), catMat);
			e[i] = energy[i] * mass / 1000. / 0.931494061;
			dedx[i] = result.dEdxi / 1000.;
		}
		tables.emplace(irisMat, make_pair(e, dedx));
	}
	#endif
}
	
