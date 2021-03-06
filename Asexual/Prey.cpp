/*
* Prey.cpp
*  - The prey objects
*/

#include "Prey.h"

#include <iostream>
#include "Ecosystem.h"
#include "FCMPrey.h"
#include <math.h>

#include <algorithm> //Reproduction based on gender /MRE/
using namespace std; //Reproduction based on gender /MRE/

Prey::Prey() {

}

Prey::~Prey() {

}

// for 1st generation individuals
Prey::Prey(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float energy, float energy_max, float speed, short speed_max, short age, short age_max, short vision, short repAge, float distanceEvol, float stateBirth, FCMPrey *model, short genderVal, float defence, float coopDefence, short persuasionVal) : Sim(type, id, species, parent1ID, parent2ID, speciesParent1ID, speciesParent2ID, energy_max, speed_max, age_max, vision, repAge, distanceEvol, stateBirth, nbGenes, genderVal, defence, coopDefence, persuasionVal) //Add Gender /MRE/
{
	setPosition(pos);
	//	calcWeight();
	setAge(age);
	setSpeed(speed);

	this->setGender(genderVal);//Add Gender /MRE/ Female=1, Male=0
	setUpdated(0);
	setLastAction(0);
	setLiving(true);

	this->StrengthStrongestCell = 0;
	this->StrengthWeakestCell = 1000000;
	this->StrengthStrongestIndivClose = 0;

	this->model = *model;
	this->model.set_prey(this);
	
	setEnergy(energy);
}
// for new born individuals  // M.M
Prey::Prey(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float speed, short age, float distanceEvol, FCMPrey *model, short genderVal) :Sim(type, id, species, parent1ID, parent2ID, speciesParent1ID, speciesParent2ID, distanceEvol, nbGenes, genderVal) //Add Gender /MRE/
{
	setPosition(pos);
	//setEnergy(energy); // M.M => because it should be calculated after specifying SOB
	setAge(age);
	setSpeed(speed);
	setUpdated(0);
	setLastAction(0);
	setLiving(true);

	this->StrengthStrongestCell = 0;
	this->StrengthWeakestCell = 10000;
	this->StrengthStrongestIndivClose = 0;

	this->model = *model;
	this->model.set_prey(this);

	this->setGender(genderVal);//Add Gender /MRE/ Female=1, Male=0
}

FCMPrey * Prey::getFCM() {
	return &model;
}


float Prey::get_mutate_amount(Ecosystem * eco){
	Random *r = eco->getRandom();
	double prob, test;
	prob = r->next(32000) / 32000.0;
	if (prob <= 0.05)
	{
		prob = r->next(32000) / 32000.0;
		if (prob <= 0.5)
			test = 0.3;
		else
			test = -0.3;
	}
	else if (prob <= 0.3)
	{
		prob = r->next(32000) / 32000.0;
		if (prob <= 0.5)
			test = 0.2;
		else
			test = -0.2;
	}
	else
	{
		prob = r->next(32000) / 32000.0;
		if (prob <= 0.5)
			test = 0.1;
		else
			test = -0.1;
	}
	return (float)test * 2; // M.M based on the changes in mutation probability (divided by 10)

}
/* RS
void Prey::reprod(Prey * friends, vector<Prey> * newPrey, Ecosystem * eco) {

	Random *r = eco->getRandom();
	short nSens = FCMPrey::nbSens;
	short nConcepts = FCMPrey::nbConcepts;
	short nMoteursDep = FCMPrey::nbMoteursDep;
	short nMoteursFix = FCMPrey::nbMoteursFix;

	short bebe_gender = eco->rand.next(0, 1); //Add Gender /MRE/ Female=1, Male=0

	FCMPrey bebe_fcm = FCMPrey(NULL);


	float distanceEvol = 0;

	Prey *selected_parent;
	float partner_select;

	// M.M commented because SOB is part of genome
	// After creating the child's genome we have sob and we can calculate energy

	// ------ Creation of FCM
	float pRemove = (float)0.0005;
	float prob;

	for (int i = 0; i < nSens + nConcepts + nMoteursDep + nMoteursFix; ++i) { //-- Generate the new Proie's concepts and chart
		partner_select = float(r->next(10000) / 10000.0);

		if (i < nSens){//MRE RandomGoodGene //get sense features from mother or father (the one with same gender)
			if (this->getGender() == bebe_gender)//female
			{
				selected_parent = this;
			}
			else
			{
				selected_parent = friends;
			}
		}
		else{
			if (partner_select < 0.5)
				selected_parent = this;
			else
				selected_parent = friends;
		}

		for (int j = 0; j < nConcepts + nMoteursDep + nMoteursFix; ++j) {
			float do_mutate = float(r->next(32000) / 32000.0);

			if (selected_parent->model.get_chart(i, j) != 0){	//A.G
				float mutate_amount = 0.0;
				if (this->getAge() > (this->getAgeMax(false) - this->getRepAge()))	//A.G
				{
					if (do_mutate <= pRemove)
					{
						bebe_fcm.set_chart(i, j, 0);
					}
					else mutate_amount = (do_mutate <= eco->getProbaMut_HighAge() + pRemove) ? get_mutate_amount(eco) : 0;	//A.G
				}
				else
				{
					if (do_mutate <= pRemove)
					{
						bebe_fcm.set_chart(i, j, 0);
					}
					else mutate_amount = (do_mutate <= eco->getProbaMut() + pRemove) ? get_mutate_amount(eco) : 0;
				}
				bebe_fcm.set_chart(i, j, selected_parent->model.get_chart(i, j) + (float)mutate_amount);
			}
			else {
				float mutate_amountLow = 0.0;
				if (this->getAge() > (this->getAgeMax(false) - this->getRepAge()))	//A.G
				{
					mutate_amountLow = (do_mutate <= eco->getProbaMutLow_HighAge()) ? get_mutate_amount(eco) : 0;	//A.G
				}
				else
				{
					mutate_amountLow = (do_mutate <= eco->getProbaMutLow()) ? get_mutate_amount(eco) : 0;
				}
				bebe_fcm.set_chart(i, j, (float)mutate_amountLow);
			}
		}
	}

	distanceEvol = bebe_fcm.calculDistanceEvol(eco->fcm_prey); // Evolution Distance of new individual

	for (int i = 0; i < nSens + nConcepts + nMoteursDep + nMoteursFix; ++i) {
		// bebe_fcm.set_activation(i, model.get_activations()->at(i));
		bebe_fcm.set_activation(i, 0); //-- Meisam: concepts should be zero?
	}


	//-- Create the new Prey
	short bebe_type = 0; //-- Prey
	long bebe_id = eco->maxPreyID;
	int bebe_species = 0;
	long bebe_parent1ID = getID();
	long bebe_parent2ID = friends->getID();
	int bebe_speciesParent1ID = getSpecies();
	int bebe_speciesParent2ID = friends->getSpecies();
	Position bebe_position = *(getPosition());
	float bebe_speed = 0;
	short bebe_age = 0;
	float bebe_distanceEvol = distanceEvol;

	// create new individual
	Prey * bebe = new Prey(bebe_type, bebe_id, bebe_species, bebe_parent1ID, bebe_parent2ID, bebe_speciesParent1ID, bebe_speciesParent2ID, bebe_position, bebe_speed, bebe_age, bebe_distanceEvol, &bebe_fcm, bebe_gender);//Add Gender /MRE/

	//-- species adjustment: For every new Prey, compare distanceEvol with parent species - add to closest species
	float distFromP1 = 0.0;
	float distFromP2 = 0.0;

	// physical gene crossover and mutation / M.M
	if (this->getGender() == 1)//female. first parameter should be mother //MRE RandomGoodGene
		bebe->phGenome.crossover(this->phGenome.getGenome(), friends->phGenome.getGenome(), bebe_gender, this->phGenome.persuasion);
	else
		bebe->phGenome.crossover(this->phGenome.getGenome(), friends->phGenome.getGenome(), bebe_gender, friends->phGenome.persuasion);

	bebe->phGenome.mutate(eco->getProbaMut(), eco);
	//GENETYPE genome_evolDistance=bebe->phGenome.calcDistance(eco->ini_PreyGenome.getGenome()); // M.M calculate evolution distance
	float genome_evolDistance;
	if (bebe->getGender() == 1){//MRE RandomGoodGene
		genome_evolDistance = bebe->phGenome.calcDistance(&eco->ini_FemalePreyGenome); // M.M calculate evolution distance
	}
	else{
		genome_evolDistance = bebe->phGenome.calcDistance(&eco->ini_MalePreyGenome); // M.M calculate evolution distance
	}

	bebe->set_DistEvolution(genome_evolDistance + distanceEvol);  //  M.M update distance evolution

	int percent = r->next(-25, 25);

	
	eco->statWorld.incAvgEnergyTransferredMalePrey((friends->getStateBirth() * friends->getEnergyMax()) / 100);
	eco->statWorld.incAvgEnergyTransferredFemalePrey((this->getStateBirth() * this->getEnergyMax()) / 100);
	bebe->setEnergy(calculationEnergyNewBorn(friends->getStateBirth(), friends->getEnergyMax(), this->getStateBirth(), this->getEnergyMax()));  // M.M
	//    bebe->calcWeight();
	float energy = bebe->getEnergy();  // M.M
	
	float geneticDistFromP1, geneticDistFromP2;  //M.M
	distFromP1 = bebe->getFCM()->calculDistanceEvol(eco, getFCM());
	//geneticDistFromP1= bebe->phGenome.calcDistance(this->phGenome.getGenome()); // M.M
	geneticDistFromP1 = bebe->phGenome.calcDistance(&this->phGenome); // M.M
	distFromP1 += geneticDistFromP1;

	distFromP2 = bebe->getFCM()->calculDistanceEvol(eco, friends->getFCM());
	//geneticDistFromP2= bebe->phGenome.calcDistance(friends->phGenome.getGenome()); // M.M
	geneticDistFromP2 = bebe->phGenome.calcDistance(&friends->phGenome); // M.M
	distFromP2 += geneticDistFromP2; // M.M

	if (distFromP1 < distFromP2) {
		bebe->setSpecies(getSpecies());
		bebe->speciesPtr = this->speciesPtr;
	}
	else {
		bebe->setSpecies(friends->getSpecies());
		bebe->speciesPtr = friends->speciesPtr;
	}

	bebe->set_action_offset(-1);
	newPrey->push_back(*bebe);	//-- Add the bebe to newPrey
	
	eco->maxPreyID++;
	eco->incNbPrey();

	//-- Adjust the the parent's energy and other properties

	incNbReprod();
	friends->incNbReprod();
	friends->setUpdated(1);

	friends->setLastAction(FCMPrey::ReproduceAction); //-- Action of reproduction

	friends->model.ConceptIndex = FCMPrey::SearchPartner;       //-- shows Concept that should changes its' value after acting
	friends->model.ConceptMultiplier = 0;  //-- Based on the model.ConceptIndex, the value of this concpet should multipy to model.ConceptMultiplier

	friends->cntReprodFailedTries = 0;
	friends->reproduceFailed1 = 0;  //-- 1: Energy of prey is not enough, 10: partner has not found, 11: both of them
	friends->reproduceFailed2 = 0;  //-- "energy of partner is not sufficient"
	friends->reproduceFailed3 = 0;  //-- "Partner has acted befor"
	friends->reproduceFailed4 = 0;  //-- "Partner has chosen another action"
	friends->reproduceFailed5 = 0;  //-- "Distance is far"
	//friends->reproduceFailed6 = 0;  //-- "Same gender" //Since Females are initiating reproduction, it happens when mate is male //MRE

	friends->set_DistMating(get_DistMating());    //-- Initial value for Distance Parents
	friends->setSpeed(0);

	eco->ParentPrey[getID()].energy = getEnergy();						//-- Meisam: Information for Parents
	eco->ParentPrey[friends->getID()].energy = friends->getEnergy();  //-- Meisam: Information for Parents

	// in first pregnancy mother spend more energy than child earn (5%)
	if (getNbReprod() == 1){
		this->setEnergy(calculationEnergyNewParent(this->getEnergy(), energy, this->getStateBirth(), friends->getStateBirth()) - FirstPregnancyEneryWaste*getEnergy());			//-- Losing Energy for reproducing
	}
	else{
		this->setEnergy(calculationEnergyNewParent(this->getEnergy(), energy, this->getStateBirth(), friends->getStateBirth()));			//-- Losing Energy for reproducing
	}
	friends->setEnergy(calculationEnergyNewParent(friends->getEnergy(), energy, friends->getStateBirth(), this->getStateBirth()));
	
	float newEnergy = friends->calculationEnergyAction(friends->getEnergy(), friends->model.getNbArc(), friends->getSpeed(), friends->getStrength(), 0, eco); //-- Losing Energy for acting
	friends->setEnergy(newEnergy);

	//-- clear the memory of Partner
	friends->friendsClose.clear();
	friends->friendsCloseDistance.clear();
	friends->opponentsClose.clear();
	friends->opponentsCloseDistance.clear();
	friends->foodClose.clear();
	friends->foodCloseDistance.clear();
	delete bebe;				//-- De-allocate

}
*/
void Prey::reprodAsexual(vector<Prey> * newPrey, Ecosystem * eco) { //RS

	Random *r = eco->getRandom();
	short nSens = FCMPrey::nbSens;
	short nConcepts = FCMPrey::nbConcepts;
	short nMoteursDep = FCMPrey::nbMoteursDep;
	short nMoteursFix = FCMPrey::nbMoteursFix;

	short bebe_gender = eco->rand.next(0, 1); //Add Gender /MRE/ Female=1, Male=0

	FCMPrey bebe_fcm = FCMPrey(NULL);


	float distanceEvol = 0;

	Prey *selected_parent = this;

	// M.M commented because SOB is part of genome
	// After creating the child's genome we have sob and we can calculate energy

    // ------ Creation of FCM
    float pRemove = (float)0.0005;
    float prob;

	for (int i = 0; i < nSens + nConcepts + nMoteursDep + nMoteursFix; ++i) { //-- Generate the new Prey's concepts and chart
		for (int j = 0; j < nConcepts + nMoteursDep + nMoteursFix; ++j) {
			float do_mutate = float(r->next(32000) / 32000.0);

			if (selected_parent->model.get_chart(i, j) != 0){	//A.G
				float mutate_amount = 0.0;
				if (this->getAge() >(this->getAgeMax(false) - this->getRepAge()))	//A.G
				{
					if (do_mutate <= pRemove)
					{
						bebe_fcm.set_chart(i, j, 0);
					}
					else mutate_amount = (do_mutate <= eco->getProbaMut_HighAge() + pRemove) ? get_mutate_amount(eco) : 0;	//A.G
				}
				else
				{
					if (do_mutate <= pRemove)
					{
						bebe_fcm.set_chart(i, j, 0);
					}
					else mutate_amount = (do_mutate <= eco->getProbaMut() + pRemove) ? get_mutate_amount(eco) : 0;
				}
				bebe_fcm.set_chart(i, j, selected_parent->model.get_chart(i, j) + (float)mutate_amount);
			}
			else {
				float mutate_amountLow = 0.0;
				if (this->getAge() > (this->getAgeMax(false) - this->getRepAge()))	//A.G
				{
					mutate_amountLow = (do_mutate <= eco->getProbaMutLow_HighAge()) ? get_mutate_amount(eco) : 0;	//A.G
				}
				else
				{
					mutate_amountLow = (do_mutate <= eco->getProbaMutLow()) ? get_mutate_amount(eco) : 0;
				}
				bebe_fcm.set_chart(i, j, (float)mutate_amountLow);
			}
		}
	}
	distanceEvol= bebe_fcm.calculDistanceEvol(eco->fcm_prey); // Evolution Distance of new individual

	for (int i = 0; i < nSens + nConcepts + nMoteursDep + nMoteursFix; ++i) {
		// bebe_fcm.set_activation(i, model.get_activations()->at(i));
		bebe_fcm.set_activation(i, 0); //-- Meisam: concepts should be zero?
	}


	//-- Create the new Prey
	short bebe_type = 0; //-- Prey
	long bebe_id = eco->maxPreyID;
	int bebe_species = 0;
	long bebe_parent1ID = getID();
	long bebe_parent2ID = getID();		//RS ...both parents = same ID => child is born asexually
	int bebe_speciesParent1ID = getSpecies();
	int bebe_speciesParent2ID = getSpecies();		//RS ...both parent species = same ID
	Position bebe_position = *(getPosition());
	float bebe_speed = 0;
	short bebe_age = 0;
	float bebe_distanceEvol = distanceEvol;

	// create new individual
	Prey * bebe = new Prey(bebe_type, bebe_id, bebe_species, bebe_parent1ID, bebe_parent2ID, bebe_speciesParent1ID, bebe_speciesParent2ID, bebe_position, bebe_speed, bebe_age, bebe_distanceEvol, &bebe_fcm, bebe_gender);//Add Gender /MRE/

	//-- species adjustment: For every new Prey, compare distanceEvol with parent species - add to closest species
	float distFromP1 = 0.0;

	// physical gene crossover and mutation / M.M
	bebe->phGenome.crossover(this->phGenome.getGenome(), this->phGenome.getGenome(), bebe_gender, this->phGenome.persuasion);		//RS

	bebe->phGenome.mutate(eco->getProbaMut(), eco);
	//GENETYPE genome_evolDistance=bebe->phGenome.calcDistance(eco->ini_PreyGenome.getGenome()); // M.M calculate evolution distance
	float genome_evolDistance;
	if (bebe->getGender() == 1){//MRE RandomGoodGene
		genome_evolDistance = bebe->phGenome.calcDistance(&eco->ini_FemalePreyGenome); // M.M calculate evolution distance
	}
	else{
		genome_evolDistance = bebe->phGenome.calcDistance(&eco->ini_MalePreyGenome); // M.M calculate evolution distance
	}

	bebe->set_DistEvolution(genome_evolDistance + distanceEvol);  //  M.M update distance evolution

	int percent = r->next(-25, 25);
	

	bebe->setEnergy(calculationEnergyNewBornAsexual(this->getStateBirth(), this->getEnergyMax()));  // RS
	//    bebe->calcWeight();
	float energy = bebe->getEnergy();  // M.M
	
	eco->statWorld.incAvgEnergyTransferredFemalePrey(energy);
    ////////////
	float geneticDistFromP1;  //M.M //RS
	distFromP1 = bebe->getFCM()->calculDistanceEvol(eco, getFCM());
	//geneticDistFromP1= bebe->phGenome.calcDistance(this->phGenome.getGenome()); // M.M
	geneticDistFromP1= bebe->phGenome.calcDistance(&this->phGenome); // M.M
	distFromP1 += geneticDistFromP1;

	bebe->setSpecies(getSpecies());
	bebe->speciesPtr = this->speciesPtr;

	bebe->set_action_offset(-1);
	newPrey->push_back(*bebe);	//-- Add the bebe to newPrey
	delete bebe;				//-- De-allocate

	eco->maxPreyID++;
	eco->incNbPrey();

	//-- Adjust the the parent's energy and other properties

	incNbReprod();

	eco->ParentPrey[getID()].energy = getEnergy();						//-- Meisam: Information for Parents

	// in first pregnancy mother spend more energy than child earn (5%)
	
	if (getNbReprod() == 1){
		this->setEnergy(calculationEnergyNewParentAsexual(this->getEnergy(), energy, this->getStateBirth()) - FirstPregnancyEneryWaste*getEnergy());			//-- Losing Energy for reproducing
	}
	else{
		this->setEnergy(calculationEnergyNewParentAsexual(this->getEnergy(), energy, this->getStateBirth()));			//-- Losing Energy for reproducing
	}
	
}

/*
* Method updateNew
* 	@Parameters
* 	- num, the current prey's position in rabbits data structure
*  - *eco, pointer to the ecosystem object
*  @Purpose
*  - computes a list of the closet food, friends, and predators
*    used to determine which action to take
*  - prey looks in a spiral direction outward from its current position
*  @Note
*  - symantics (1, 2, 3, 4) == (North, East, South, West)
*  - lookPermut[1..8] = {(1,2,3,4), (2,3,4,1), (3,4,1,2), (4,1,2,3),
*  				  (1,4,3,2), (4,3,2,1), (3,2,1,4), (2,1,4,3)}
*/
void Prey::updateNew(Ecosystem * eco) {

	//-- Clear the current list of friends and distance to friends
	friendsClose.clear();
	friendsCloseDistance.clear();

	//-- Clear the current list of predators and distance to predators
	opponentsClose.clear();
	opponentsCloseDistance.clear();

	//-- Clear the current food and distance to food
	foodClose.clear();
	foodCloseDistance.clear();

	// A.G
	// clear variables
	setStrengthStrongestIndivCloseIndex(-1);
	setStrengthStrongestIndivClose(0);

	Position pos_temp;
	pos_temp.x = -1;
	pos_temp.y = -1;
	setStrengthStrongestCellPos(pos_temp);
	setStrengthStrongestCell(0);

	setStrengthWeakestCellPos(pos_temp);
	setStrengthWeakestCell(100000);


#ifdef TWO_RESOURCES
	foodClose2.clear();
	foodCloseDistance2.clear();
#endif

	//-- Select a random direction for looking to start looking
	int direction = eco->rand.next(1, 8);

	//-- First distance to look (0= present cell) but these functins can't calculate "distance = 0"
	int distance = 1;

	//-- Number of Local Friends, Enemies & Foods
	const int nbList = 5;

	//-- Counters
	int nbFriends = 0;
	int nbOpponents = 0;
	int nbFoods = 0;
#ifdef TWO_RESOURCES
	int nbFoods2 = 0;
#endif

#ifdef TWO_RESOURCES
	getListFriends(nbList, &distance, direction, &nbFriends, &nbOpponents, &nbFoods, &nbFoods2, eco);
#else
	getListFriends(nbList, &distance, direction, &nbFriends, &nbOpponents, &nbFoods, eco);
#endif
	cout << flush;

	//-- Action: 0-escape, 1-rsearchFood, 2-socialize, 3-explore, 4-stay, 5-eat, 6-reproduce
	short na = model.percept_prey(eco);
	set_action_offset(na);
}

/*
* Uses distance and direction to look around itself in the world
* Finds the closest friends and adds them to the vector friendsClose
*/
#ifdef TWO_RESOURCES
void Prey::getListFriends(int nbList, int * distance, int direction, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco) {

	while ((*distance < getVision()) && (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)) {
		switch (direction) {
		case 1: {
					partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		case 2: {
					partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		case 3: {
					partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		case 4: {
					partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		case 5: {
					partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		case 6: {
					partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		case 7: {
					partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		case 8: {
					partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList || *nbFoods2 < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco);
					break;
		}

		default:
			break;
		} //-- Endof switch

		(*distance)++;

	} //-- End of while

}
#else
void Prey::getListFriends(int nbList, int * distance, int direction, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {
	while ((*distance < getVision()) && (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)) {
		switch (direction) {
		case 1: {
					partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 2: {
					partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 3: {
					partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 4: {
					partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 5: {
					partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 6: {
					partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 7: {
					partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 8: {
					partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		default:
			break;
		} //-- Endof switch

		(*distance)++;

	} //-- End of while

}

#endif

/*
* Finds the friends in the north direction
*/

#ifdef TWO_RESOURCES
void Prey::partialCompute1(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco) {
#else
void Prey::partialCompute1(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {
#endif
	Position pTemp;

	//-- Look in one row
	int i = 0;
	for (int x = -(*distance - 1); x <= *distance; x++) {
		i++;
		if (i == 26)
			i = 2;
		//-- ChangePosition :
		//--		pTemp.x = position->x + x;
		//--		pTemp.y = position->y - *distance;

		pTemp = Movement(x, -*distance, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
#ifdef TWO_RESOURCES
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco, &pTemp);
#else
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
#endif
	}
}

/*
* Finds the friends in the east direction
*/
#ifdef TWO_RESOURCES
void Prey::partialCompute2(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco) {
#else
void Prey::partialCompute2(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {
#endif

	Position pTemp;

	//-- Look in one column
	for (int y = -(*distance - 1); y <= *distance; y++) {

		//-- ChangePosition :
		//--		pTemp.x = position->x + *distance;
		//--		pTemp.y = position->y + y;

		pTemp = Movement(*distance, y, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
#ifdef TWO_RESOURCES
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco, &pTemp);
#else
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
#endif
	}
}

/*
* Finds the friends in the south direction
*/
#ifdef TWO_RESOURCES
void Prey::partialCompute3(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco) {
#else
void Prey::partialCompute3(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {
#endif

	Position pTemp;

	//-- Look in one row
	for (int x = (*distance - 1); x >= (-*distance); x--) {

		//-- ChangePosition :
		//--		pTemp.x = position->x + x;
		//--		pTemp.y = position->y + *distance;;

		pTemp = Movement(x, *distance, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
#ifdef TWO_RESOURCES
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco, &pTemp);
#else
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
#endif
	}
}

/*
* Finds the friends in the west direction
*/
#ifdef TWO_RESOURCES
void Prey::partialCompute4(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco) {
#else
void Prey::partialCompute4(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {
#endif

	Position pTemp;

	//-- Look in one column
	for (int y = (*distance - 1); y >= (-*distance); y--) {

		//-- ChangePosition :
		//--		pTemp.x = position->x - *distance;
		//--		pTemp.y = position->y + y;

		pTemp = Movement(-*distance, y, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
#ifdef TWO_RESOURCES
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, nbFoods2, eco, &pTemp);
#else
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
#endif
	}
}

#ifdef TWO_RESOURCES
void Prey::lookat(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco, Position * pTemp) {
#else
void Prey::lookat(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco, Position * pTemp) {
#endif

	int FriendType = 0;   //-- Prey
	int OpponentType = 1; //-- Predator
	float FoodAvailability = eco->getLevelGrass(*pTemp);
#ifdef TWO_RESOURCES
	float FoodAvailability2 = eco->getLevelGrass2(*pTemp);
#endif


	if (*nbFriends < nbList) {
		//-- Get list of Friends in the Cell pTemp
		vector<int> nearbyFriends = *(eco->getListCreature(*pTemp, FriendType));

		//-- Continue until (1) found nbList friends, or (2) ran out of nearby friends -> add to list of friends
		while (*nbFriends < nbList && int(nearbyFriends.size()) > 0) {
			//-- Choose random friend to add to list
			int fID = eco->rand.next(0, int(nearbyFriends.size()) - 1);
			//-- Add distance value of friend in to the Global list friendsCloseDistance
			friendsCloseDistance.insert(friendsCloseDistance.begin(), *distance);
			//-- Add position of friend based on the distance in to Global list friendsClose
			friendsClose.insert(friendsClose.begin(), nearbyFriends.at(fID));
			//-- Remove the friend from the list of available friends
			nearbyFriends.erase(nearbyFriends.begin() + fID);
			*nbFriends += 1;
		}
	}

	if (*nbOpponents < nbList) {
		//-- Get list of Opponents in the Cell pTemp
		vector<int> nearbyOpponents = *(eco->getListCreature(*pTemp, OpponentType));

		//-- Continue until (1) found nbList Opponents, or (2) ran out of nearby Opponents -> add to list of Opponents
		while (*nbOpponents < nbList && int(nearbyOpponents.size()) > 0) {
			int fID = eco->rand.next(0, int(nearbyOpponents.size()) - 1);
			//-- Add distance value of Opponent into the Global list OpponentsCloseDistance
			opponentsCloseDistance.insert(opponentsCloseDistance.begin(), *distance);
			//-- Add position of Opponent into the Global list OpponentsClose
			opponentsClose.insert(opponentsClose.begin(), nearbyOpponents.at(fID));
			//-- Remove the Opponent from the list of available Opponents
			nearbyOpponents.erase(nearbyOpponents.begin() + fID);
			*nbOpponents += 1;
		}
	}

	if (*nbFoods < nbList && FoodAvailability >= 1) {
		//-- Add distance value of food into the Global list foodCloseDistance
		foodCloseDistance.insert(foodCloseDistance.begin(), *distance);
		//-- Add position of food into the Global list foodClose
		foodClose.insert(foodClose.begin(), *pTemp);
		*nbFoods += 1;
	}


	// =============	A.G //Move2WeakestPreyCell //MRE RandomGoodGene
	if (*nbFriends > 0)
	{
		//-- Get list of Friends in the Cell pTemp
		vector<int> nearbyFriends = *(eco->getListCreature(*pTemp, FriendType));
		float sum1 = 0;
		float sum2 = 0;//added for StrengthStrongestPreyCellDistance(High/Low)
		for (int cnt = 0; cnt < (int)nearbyFriends.size(); cnt++)
		{
			if ((eco->rabbits[nearbyFriends[cnt]].getGender()) == this->getGender()){
				sum1 = sum1 + eco->rabbits[nearbyFriends[cnt]].getStrength();
			}
			else{
				sum2 = sum2 + eco->rabbits[nearbyFriends[cnt]].getStrength();
			}


			if (eco->rabbits[nearbyFriends[cnt]].getStrength() > getStrengthStrongestIndivClose() && eco->rabbits[nearbyFriends[cnt]].getGender() != this->getGender()) //Move2StrongestPrey
			{
				setStrengthStrongestIndivCloseIndex(nearbyFriends[cnt]);
				setStrengthStrongestIndivClose(eco->rabbits[nearbyFriends[cnt]].getStrength());
			}
		}

		if (sum2 > getStrengthStrongestCell())
		{
			setStrengthStrongestCellPos(*pTemp);//StrengthStrongestPreyCellDistance(High/Low) ?
			setStrengthStrongestCell(sum2);
		}
		if (sum1 < getStrengthWeakestCell())
		{
			setStrengthWeakestCellPos(*pTemp); //Move2WeakestPreyCell
			setStrengthWeakestCell(sum1);
		}
	}

	// =========


#ifdef TWO_RESOURCES
	if (*nbFoods2 < nbList && FoodAvailability2 >= 1) {
		//-- Add distance value of food into the Global list foodCloseDistance
		foodCloseDistance2.insert(foodCloseDistance2.begin(), *distance);
		//-- Add position of food into the Global list foodClose
		foodClose2.insert(foodClose2.begin(), *pTemp);
		*nbFoods2 += 1;
	}
#endif


}


void Prey::act_driver(vector<Prey> * newPrey, Ecosystem * eco, short MatingMode) {
	Position newPos;

	setUpdated(1);

	eco->ParentPrey[getID()].energy = getEnergy(); //-- Meisam: Information for Parents

	newPos = act(newPrey, eco, MatingMode);
	setPosition(newPos);
	if (this->getGender() == 0){
		float newEnergy = calculationEnergyAction(getEnergy(), model.getNbArc(), getSpeed(), getStrength(), 0, eco);  //-- Losing Energy for acting //RS males don't carry persuasion penalty
		setEnergy(newEnergy);
	}
	else{
		float newEnergy = calculationEnergyAction(getEnergy(), model.getNbArc(), getSpeed(), getStrength(), getPersuasion(), eco);  //-- Losing Energy for acting	//RS females do carry persuasion penalty
		setEnergy(newEnergy);
	}
	//-- clear the memory
	friendsClose.clear();
	friendsCloseDistance.clear();
	opponentsClose.clear();
	opponentsCloseDistance.clear();
	foodClose.clear();
	foodCloseDistance.clear();
	// A.G
	setStrengthStrongestIndivCloseIndex(-1);
	setStrengthStrongestIndivClose(0);

	Position pos_temp;
	pos_temp.x = -1;
	pos_temp.y = -1;
	setStrengthStrongestCellPos(pos_temp);
	setStrengthStrongestCell(0);

	setStrengthWeakestCellPos(pos_temp);
	setStrengthWeakestCell(100000);


#ifdef TWO_RESOURCES
	foodClose2.clear();
	foodCloseDistance2.clear();
#endif
}


Position Prey::act(vector<Prey> * newPrey, Ecosystem * eco, short MatingMode) {
	//-- Meisam, Feb 2011: Escape, Search, Socialize, Explore & Reproduce have changed.
	//--   Lists of friendsClose, opponentsClose, ... do not contain individuals in a current cell (distance=0)
	Random *r = eco->getRandom();

	cout << flush;
	Position posNouvelle;
	posNouvelle.x = 0;
	posNouvelle.y = 0;
	Position posRelative;
	posRelative.x = 0;
	posRelative.y = 0;

	Direction dir;
	dir.x = 0;
	dir.y = 0;

	short VisionMe = 5;
	float e = getEnergy();
	float distance;
	model.ConceptIndex = 0;       //-- shows Concept that should changes its' value after acting
	model.ConceptMultiplier = 1;  //-- Based on the model.ConceptIndex, the value of this concpet should multipy to model.ConceptMultiplier

	cntReprodFailedTries = 0;
	reproduceFailed1 = 0;  //-- 1: Energy of prey is not enough, 10: partner has not found, 11: both of them
	reproduceFailed2 = 0;  //-- "energy of partner is not sufficient"
	reproduceFailed3 = 0;  //-- "Partner has acted befor"
	reproduceFailed4 = 0;  //-- "Partner has chosen another action"
	reproduceFailed5 = 0;  //-- "Distance is far"
	//reproduceFailed6 = 0;  //-- "Same gender" //Since Females are initiating reproduction, it happens when mate is male //MRE

	set_DistMating(0);    //-- Initial value for Distance Parents

	int ao = get_action_offset();
	switch (ao) {

	case FCMPrey::Escape: { //-- escape
							  //-- Meisam, Feb 2011: 1)was rewrited. 2)Escape from the average of all individuals in opponentsCloseDistance
							  //--                   3)Random escape was changed

							  if (int(opponentsClose.size()) == 0) { //-- Vicinity hasn't any predators then random walk
								  if (int(eco->getListCreature(*(getPosition()), 1)->size()) > 0) { //-- Current cell has predator
									  dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random escape
									  dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random escape
								  }
								  //else { //-- Current cell & Vicinity haven't any predators
								  // dir.x = VisionMe - 3 - r->next(2 * VisionMe - 7); //-- (8 - r->next(17)); : random escape
								  // dir.y = VisionMe - 3 - r->next(2 * VisionMe - 7); //-- (8 - r->next(17)); : random escape
								  //}
							  }
							  else { //-- calculate the avarege of distance of individual from all predators
								  int TmpDist = 0;
								  int TmpdistMax;
								  int TmpDistConv;
								  Position posTMP;
								  float posAverage_x = 0;
								  float posAverage_y = 0;

								  TmpdistMax = opponentsCloseDistance[0]; //-- Dist of Farthest local predator
								  //TmpdistMin = opponentsCloseDistance[int (opponentsCloseDistance.size())-1]; //-- Dist of Nearest local predator

								  //-- Online-Average position of all enemies in vicinity based on weighted Distance
								  for (int i = 0; i < int(opponentsCloseDistance.size()); i++){
									  //TmpDistConv = abs(opponentsCloseDistance[i]-TmpdistMax) + TmpdistMin; //-- convert dist max to min -> abs(x-Max)-Min
									  TmpDistConv = abs(opponentsCloseDistance[i] - (TmpdistMax + 1)); //-- convert dist max to min -> abs(x-Max-1)
									  posTMP = *(eco->wolves[opponentsClose[i]].getPosition());
									  TmpDist = TmpDist + TmpDistConv;
									  if (posTMP.x >(eco->getWidth() - posTMP.x))  //-- Unifying positions in the boundries of world
										  posTMP.x = eco->getWidth() - posTMP.x;
									  if (posTMP.y > (eco->getHeight() - posTMP.y)) //-- Unifying positions in the boundries of world
										  posTMP.y = eco->getHeight() - posTMP.y;
									  posAverage_x = (((TmpDist - TmpDistConv) * posAverage_x) + (TmpDistConv*posTMP.x)) / TmpDist;
									  posAverage_y = (((TmpDist - TmpDistConv) * posAverage_y) + (TmpDistConv*posTMP.y)) / TmpDist;
								  }

								  ////-- Online-Average position of all enemies in current cell based on weighted Distance
								  ////TmpDistConv = abs(0-TmpdistMax) + TmpdistMin; //-- convert dist max to min -> abs(x-Max)-Min
								  //TmpDistConv = abs(0 - (TmpdistMax + 1)); //-- convert dist max to min -> abs(x-Max-1)
								  //for (int i = 0; i < int(eco->getListCreature(*(getPosition()), 1)->size()); i++){
								  // posTMP = *(eco->wolves[(*eco->getListCreature(*(getPosition()), 1))[i]].getPosition());
								  // TmpDist = TmpDist + TmpDistConv;
								  // if (posTMP.x >(eco->getWidth() - posTMP.x))  //-- Unifying positions in the boundries of world
								  //  posTMP.x = eco->getWidth() - posTMP.x;
								  // if (posTMP.y > (eco->getHeight() - posTMP.y)) //-- Unifying positions in the boundries of world
								  //  posTMP.y = eco->getHeight() - posTMP.y;
								  // posAverage_x = (((TmpDist - TmpDistConv) * posAverage_x) + (TmpDistConv*posTMP.x)) / TmpDist;
								  // posAverage_y = (((TmpDist - TmpDistConv) * posAverage_y) + (TmpDistConv*posTMP.y)) / TmpDist;
								  //}
								  if (fabs(getPosition()->x - posAverage_x) > fabs(getPosition()->x - (eco->getWidth() - posAverage_x)))
									  posAverage_x = eco->getWidth() - posAverage_x; //-- Real average of positions based on
								  if (fabs(getPosition()->y - posAverage_y) > fabs(getPosition()->y - (eco->getHeight() - posAverage_y)))
									  posAverage_y = eco->getHeight() - posAverage_y;
								  Position posAverage;
								  posAverage.x = round((posAverage_x));
								  posAverage.y = round((posAverage_y));
								  dir = getPosition()->calculDirection(posAverage, eco->getWidth(), eco->getHeight());
								  dir = dir.inverse();
							  }

							  posRelative.x = dir.x;
							  posRelative.y = dir.y;
							  distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

							  //-- Meisam
							  float ActionActivation = model.get_activations()->at(ao);
							  if (fabs(ActionActivation) > 0)
								  distance = distance / (ActionActivation * getSpeedMax());
							  else
								  distance = 0;

							  if (distance > 0) {
								  posRelative.x = int(posRelative.x / distance);
								  posRelative.y = int(posRelative.y / distance);
							  }
							  else {
								  posRelative.x = 0;
								  posRelative.y = 0;
							  }

							  posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

							  model.ConceptIndex = FCMPrey::Fear;		 //-- Meisam
							  model.ConceptMultiplier = float(1 / 2.0); //-- Meisam

							  setLastAction(FCMPrey::EscapeAction);
							  break;
	}

	case FCMPrey::SearchFood: {//-- search food
								  //-- Meisam, Feb 2011: 1)was rewrited. 2)The condition of "no food in vicinity" was added

								  if (int(foodClose.size()) > 0){
									  dir = getPosition()->calculDirection(foodClose[int(foodClose.size()) - 1], eco->getWidth(), eco->getHeight());
								  }
								  else{ //-- There isn't any food in the vicinity -> random search
									  dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random search
									  dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21));
								  }
								  posRelative.x = dir.x;
								  posRelative.y = dir.y;
								  distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

								  //-- Meisam
								  float ActionActivation = model.get_activations()->at(ao);
								  if (fabs(ActionActivation) > 0)
									  distance = distance / (ActionActivation * getSpeedMax());
								  else
									  distance = 0;

								  if (distance > 1){
									  posRelative.x = int(posRelative.x / distance);
									  posRelative.y = int(posRelative.y / distance);
									  setLastAction(FCMPrey::SearchFoodFailAction);  //-- Search for food is failed because the distance is far
								  }
								  else
								  if (int(foodClose.size()) > 0)
									  setLastAction(FCMPrey::SearchFoodAction);
								  else
								  if (distance > 0){ //-- if ( (int)food.size() == 0) -> No food then random search
									  posRelative.x = int(posRelative.x / distance);
									  posRelative.y = int(posRelative.y / distance);
									  setLastAction(FCMPrey::SearchFoodFailAction); //-- Search for food is failed
								  }
								  else { //-- distance == 0
									  posRelative.x = 0;
									  posRelative.y = 0;
									  setLastAction(FCMPrey::SearchFoodFailAction); //-- Search for food is failed
								  }

								  posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

								  break;
	}

#ifdef TWO_RESOURCES
	case FCMPrey::SearchFood2: {//-- search Food2
								   //-- Meisam, Feb 2011: 1)was rewrited. 2)The condition of "no Food2 in vicinity" was added

								   if (int(foodClose2.size()) > 0){
									   dir = getPosition()->calculDirection(foodClose2[int(foodClose2.size()) - 1], eco->getWidth(), eco->getHeight());
								   }
								   else{ //-- There isn't any food in the vicinity -> random search
									   dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random search
									   dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21));
								   }
								   posRelative.x = dir.x;
								   posRelative.y = dir.y;
								   distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

								   //-- Meisam
								   float ActionActivation = model.get_activations()->at(ao);
								   if (fabs(ActionActivation) > 0)
									   distance = distance / (ActionActivation * getSpeedMax());
								   else
									   distance = 0;

								   if (distance > 1){
									   posRelative.x = int(posRelative.x / distance);
									   posRelative.y = int(posRelative.y / distance);
									   setLastAction(FCMPrey::SearchFoodFail2Action);  //-- Search for food is failed because the distance is far
								   }
								   else
								   if (int(foodClose2.size()) > 0)
									   setLastAction(FCMPrey::SearchFood2Action);
								   else
								   if (distance > 0){ //-- if ( (int)foodClose2.size() == 0) -> No food then random search
									   posRelative.x = int(posRelative.x / distance);
									   posRelative.y = int(posRelative.y / distance);
									   setLastAction(FCMPrey::SearchFoodFail2Action); //-- Search for food is failed
								   }
								   else { //-- distance == 0
									   posRelative.x = 0;
									   posRelative.y = 0;
									   setLastAction(FCMPrey::SearchFoodFail2Action); //-- Search for food is failed
								   }

								   posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

								   break;
	}
#endif

	case FCMPrey::Socialize: { //-- sociabilisation
								 //-- Meisam, Feb 2011: 1)was rewrited. 2)Random walk wass changed

								 if (int(friendsClose.size()) > 0) { //-- goes toward friendsClose
									 dir = getPosition()->calculDirection(*(eco->rabbits[friendsClose[int(friendsClose.size()) - 1]].getPosition()), eco->getWidth(), eco->getHeight());
								 }
								 else { //-- There isn't any friendsCloses in the vicinity
									 dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random walk
									 dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21));
								 }
								 posRelative.x = dir.x;
								 posRelative.y = dir.y;
								 distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

								 //-- Meisam
								 float ActionActivation = model.get_activations()->at(ao);
								 if (fabs(ActionActivation) > 0)
									 distance = distance / (ActionActivation * getSpeedMax());
								 else
									 distance = 0;

								 if (distance > 1){
									 posRelative.x = int(posRelative.x / distance);
									 posRelative.y = int(posRelative.y / distance);
									 setLastAction(FCMPrey::SocializeFailAction); //-- Socialization failed because the distance of friendsClose is far
								 }
								 else
								 if (int(friendsClose.size()) > 0){
									 model.ConceptIndex = FCMPrey::SearchPartner;	//-- Meisam
									 model.ConceptMultiplier = float(1 / 3.0);		//-- Meisam
									 setLastAction(FCMPrey::SocializeAction);
								 }
								 else
								 if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friendsClose then random walk
									 posRelative.x = int(posRelative.x / distance);
									 posRelative.y = int(posRelative.y / distance);
									 setLastAction(FCMPrey::SocializeFailAction); //-- Socialization failed
								 }
								 else { //-- distance == 0
									 posRelative.x = 0;
									 posRelative.y = 0;
									 setLastAction(FCMPrey::SocializeFailAction); //-- Socialization failed
								 }

								 posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

								 break;
	}

		// ================== A.G
	case FCMPrey::Move2StrongestPrey: { //-- Move to strongest individual

										  if (getStrengthStrongestIndivCloseIndex() != -1) { //-- goes toward friendsClose
											  dir = getPosition()->calculDirection(*(eco->rabbits[getStrengthStrongestIndivCloseIndex()].getPosition()), eco->getWidth(), eco->getHeight());
										  }
										  else { //-- There isn't any friendsCloses in the vicinity
											  dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random walk
											  dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21));
										  }
										  posRelative.x = dir.x;
										  posRelative.y = dir.y;
										  distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

										  //
										  float ActionActivation = model.get_activations()->at(ao);
										  if (fabs(ActionActivation) > 0)
											  distance = distance / (ActionActivation * getSpeedMax());
										  else
											  distance = 0;


										  if (distance > 1){
											  posRelative.x = int(posRelative.x / distance);
											  posRelative.y = int(posRelative.y / distance);
											  setLastAction(FCMPrey::Move2StrongestPreyFailAction); //-- Socialization failed because the distance of friendsClose is far
										  }
										  else
										  {
											  if (getStrengthStrongestIndivCloseIndex() != -1){
												  model.ConceptIndex = FCMPrey::SearchPartner;
												  model.ConceptMultiplier = float(1 / 3.0);
												  setLastAction(FCMPrey::Move2StrongestPreyAction);
											  }
											  else
											  {
												  if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friendsClose then random walk
													  posRelative.x = int(posRelative.x / distance);
													  posRelative.y = int(posRelative.y / distance);
													  setLastAction(FCMPrey::Move2StrongestPreyFailAction); //-- Socialization failed
												  }
												  else { //-- distance == 0
													  posRelative.x = 0;
													  posRelative.y = 0;
													  setLastAction(FCMPrey::Move2StrongestPreyFailAction); //-- Socialization failed
												  }
											  }
										  }

										  posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());
										  break;
	}
	case FCMPrey::Move2StrongestPreyCell: { //-- Move to strongest cell

											  if (getStrengthStrongestCellPos()->x != -1) { //-- goes toward friendsClose
												  dir = getPosition()->calculDirection(*getStrengthStrongestCellPos(), eco->getWidth(), eco->getHeight());
											  }
											  else { //-- There isn't any friendsCloses in the vicinity
												  dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random walk
												  dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21));
											  }
											  posRelative.x = dir.x;
											  posRelative.y = dir.y;
											  distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

											  //-- Meisam
											  float ActionActivation = model.get_activations()->at(ao);
											  if (fabs(ActionActivation) > 0)
												  distance = distance / (ActionActivation * getSpeedMax());
											  else
												  distance = 0;

											  if (distance > 1){
												  posRelative.x = int(posRelative.x / distance);
												  posRelative.y = int(posRelative.y / distance);
												  setLastAction(FCMPrey::Move2StrongestPreyCellFailAction); //-- Socialization failed because the distance of friendsClose is far
											  }
											  else
											  {
												  if (getStrengthStrongestCellPos()->x != -1){
													  model.ConceptIndex = FCMPrey::SearchPartner;
													  model.ConceptMultiplier = float(1 / 3.0);
													  setLastAction(FCMPrey::Move2StrongestPreyCellAction);
												  }
												  else
												  {
													  if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friendsClose then random walk
														  posRelative.x = int(posRelative.x / distance);
														  posRelative.y = int(posRelative.y / distance);
														  setLastAction(FCMPrey::Move2StrongestPreyCellFailAction); //-- Socialization failed
													  }
													  else { //-- distance == 0
														  posRelative.x = 0;
														  posRelative.y = 0;
														  setLastAction(FCMPrey::Move2StrongestPreyCellFailAction); //-- Socialization failed
													  }
												  }
											  }

											  posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());
											  break;
	}
	case FCMPrey::Move2WeakestPreyCell: { //-- Move to weakest cell

											if (getStrengthWeakestCellPos()->x != -1) { //-- goes toward friendsClose
												dir = getPosition()->calculDirection(*getStrengthWeakestCellPos(), eco->getWidth(), eco->getHeight());
											}
											else { //-- There isn't any friendsCloses in the vicinity
												dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : random walk
												dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21));
											}
											posRelative.x = dir.x;
											posRelative.y = dir.y;
											distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

											//-- Meisam
											float ActionActivation = model.get_activations()->at(ao);
											if (fabs(ActionActivation) > 0)
												distance = distance / (ActionActivation * getSpeedMax());
											else
												distance = 0;

											if (distance > 1){
												posRelative.x = int(posRelative.x / distance);
												posRelative.y = int(posRelative.y / distance);
												setLastAction(FCMPrey::Move2WeakestPreyCellFailAction); //-- Socialization failed because the distance of friendsClose is far
											}
											else
											{
												if (getStrengthWeakestCellPos()->x != -1){
													model.ConceptIndex = FCMPrey::SearchPartner;
													model.ConceptMultiplier = float(1 / 3.0);
													setLastAction(FCMPrey::Move2WeakestPreyCellAction);
												}
												else
												{
													if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friendsClose then random walk
														posRelative.x = int(posRelative.x / distance);
														posRelative.y = int(posRelative.y / distance);
														setLastAction(FCMPrey::Move2WeakestPreyCellFailAction); //-- Socialization failed
													}
													else { //-- distance == 0
														posRelative.x = 0;
														posRelative.y = 0;
														setLastAction(FCMPrey::Move2WeakestPreyCellFailAction); //-- Socialization failed
													}
												}
											}


											posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());
											break;
	}
		//=====================  A.G  ==== end
	case FCMPrey::Exploration: { //-- explore
								   //-- Meisam, Feb 2011: 1)Condition "if (distance > 1)" was added
								   //--                   2)Random walk was changed
								   dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21)); : Random explore
								   dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (11 - r->next(21));

								   posRelative.x = dir.x;
								   posRelative.y = dir.y;
								   distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

								   //-- Meisam
								   float ActionActivation = model.get_activations()->at(ao);
								   if (fabs(ActionActivation) > 0)
									   distance = distance / (ActionActivation * getSpeedMax());
								   else
									   distance = 0;

								   if (distance > 0) {
									   posRelative.x = int(posRelative.x / distance);
									   posRelative.y = int(posRelative.y / distance);
								   }
								   else {
									   posRelative.x = 0;
									   posRelative.y = 0;
								   }

								   posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

								   model.ConceptIndex = FCMPrey::Curiosity;  //-- Meisam
								   model.ConceptMultiplier = float(1 / 1.5);  //-- Meisam
								   setLastAction(FCMPrey::ExplorationAction);
								   break;
	}

	case FCMPrey::Wait: { //-- stay
							posNouvelle.x = getPosition()->x;
							posNouvelle.y = getPosition()->y;
							setLastAction(FCMPrey::WaitAction);
							break;
	}

	case FCMPrey::Eat: { //-- eat
						   posNouvelle.x = getPosition()->x;
						   posNouvelle.y = getPosition()->y;

						   float ava_energy = eco->getLevelGrass(posNouvelle);  // M.M
						   //if (eco->getLevelGrass(posNouvelle) >= 1) {
						   if (ava_energy > 0) {
							   //float req_energy = fmin(this->getEnergyMax() - this->getEnergy(), this->getEnergyMax() * 0.75);  // M.M
							   float req_energy = this->getEnergyMax() - this->getEnergy();
							   if (ava_energy >= req_energy) {// available energy more than required energy  /M.M
								   eco->decLevelGrass(posNouvelle, req_energy);
								   setEnergy(this->getEnergyMax());
							   }
							   else { // available energy is not enough but still better than nothing   /M.M
								   eco->decLevelGrass(posNouvelle, ava_energy);
								   setEnergy(e + ava_energy);
							   }
							   //setEnergy(e + eco->getValueGrass());  M.M
							   //eco->decLevelGrass(posNouvelle);  M.M
							   setLastAction(FCMPrey::EatAction);
							   model.ConceptIndex = FCMPrey::Hunger;	 //-- Meisam
							   model.ConceptMultiplier = float(1 / 4.0); //-- Meisam
						   }
						   else
							   setLastAction(FCMPrey::EatFailAction);  //-- Eating failed
						   break;
	}
#ifdef TWO_RESOURCES
	case FCMPrey::Eat2: { //-- eat
							posNouvelle.x = getPosition()->x;
							posNouvelle.y = getPosition()->y;

							if (eco->getLevelGrass2(posNouvelle) >= 1) {
								setEnergy(e + eco->getValueGrass2());
								eco->decLevelGrass2(posNouvelle);
								setLastAction(FCMPrey::Eat2Action);
								model.ConceptIndex = FCMPrey::Hunger;	 //-- Meisam
								model.ConceptMultiplier = float(1 / 4.0); //-- Meisam
							}
							else
								setLastAction(FCMPrey::EatFail2Action);  //-- Eating failed
							break;
	}
#endif
/*
		case FCMPrey::Reproduce: { //-- reproduce
			//-- Meisam, Feb 2011: 1) was rewrited. 2) Mating based on the all individuals in the current cell
			posNouvelle.x = getPosition()->x;
			posNouvelle.y = getPosition()->y;

			vector<int> LocalFriends = *(eco->getListCreature(*(getPosition()), 0)); //-- list of individuals in the current cell
			int NBfriends = (int) LocalFriends.size() - 1;

			reproduceFailed1 = 0;				   //-- 1: Energy of prey is not enoug, 10: partner has not found, 11: both of them
			reproduceFailed2 = (float) NBfriends;  //-- Average of "energy of partner is not sufficient"
			reproduceFailed3 = (float) NBfriends;  //-- Average of "Partner has acted befor"
			reproduceFailed4 = (float) NBfriends;  //-- Average of "Partner has chosen another action"
			reproduceFailed5 = (float) NBfriends;  //-- Average of "Distance is far"
			reproduceFailed6 = (float) NBfriends;  //-- Average of "Same gender" //Since Females are initiating reproduction, it happens when mate is male //MRE

			reproduceFailed1 = reproduceFailed1 | ( (NBfriends <= 0) ? 10 : 00 ); //-- partner has not found

			//Reproduction based on gender /MRE/ START
			vector<Prey> LocalFriendsPrey;
			for (unsigned int i = 0; i < LocalFriends.size(); i++) { // Act based on strength M.M; who has more strength act first
					cout << flush;
					LocalFriendsPrey.push_back((Prey) eco->rabbits[LocalFriends[i]]);
			}
			sort(LocalFriendsPrey.begin(),LocalFriendsPrey.end());
			//Reproduction based on gender /MRE/ END

			bool pasFait = true;
			if (this->getStateBirth() * this->getEnergyMax() * 1. / 100) ) {
				reproduceFailed1 = reproduceFailed1 | 1;  } //-- Energy of prey is not enough
			else{
				//int k; //Reproduction based on gender /MRE/ Changed the logic of k, beacuse the list is not random anymore. we need a sorted list
				int cntrLocalFriends = 0;//MRE
				int cntReprodFailedTries = 0; //MRE RandomGoodGene

				while (pasFait && cntrLocalFriends <= int (LocalFriends.size())-1) {
                    //k = eco->rand.next(0, (int) LocalFriends.size() - 1);   //-- make randomomize order

					if (getID() != eco->rabbits[LocalFriends[cntrLocalFriends]].getID()) { //-- Preventing for mating with itself

                        short MateFlag = 5;
                        if ( eco->rabbits[LocalFriends[cntrLocalFriends]].getGender() ==  0 ) { //MRE 0 means Gender::Male
                            reproduceFailed6--;     //-- Partner is Male
                            MateFlag--;
                        }
                        if ( eco->rabbits[LocalFriends[cntrLocalFriends]].getEnergy() > (eco->rabbits[LocalFriends[cntrLocalFriends]].getEnergyMax() * 0.125) ) {
                            reproduceFailed2--;     //-- Energy of partner is not enough
                            MateFlag--;
                        }
                        if ( eco->rabbits[LocalFriends[cntrLocalFriends]].getUpdated() == 0 ) {
                            reproduceFailed3--;     //-- Partner has acted
                            MateFlag--;
                        }
                        if ( int (eco->rabbits[LocalFriends[cntrLocalFriends]].get_action_offset()) == FCMPrey::Reproduce ) {
                            reproduceFailed4--;     //-- Partner has chosen another action
                            MateFlag--;
                        }
                        //GENETYPE distance= this->phGenome.calcDistance(eco->rabbits[LocalFriends[k]].getPhGenome()->getGenome()); // M.M
                        float distance= this->phGenome.calcDistance(eco->rabbits[LocalFriends[cntrLocalFriends]].getPhGenome()); // M.M
                        if ( distance + model.calculDistanceEvol(eco, eco->rabbits[LocalFriends[cntrLocalFriends]].getFCM()) < eco->getDistanceSpeciesPrey() ) { // M.M modified
                            reproduceFailed5--;     //-- Distance is far
                            MateFlag--;
                        }
                        if ( MateFlag == 0 ) { //-- Do reproduction

							this->setEnergy(this->getEnergy() - this->getEnergy()*cntReprodFailedTries*0.01); //MRE RandomGoodGene //decrease 1% of female energy for unsuccessful mating tries

                        	//GENETYPE distance= this->phGenome.calcDistance(eco->rabbits[LocalFriends[k]].getPhGenome()->getGenome());  // M.M
                        	float distance= this->phGenome.calcDistance(eco->rabbits[LocalFriends[cntrLocalFriends]].getPhGenome());  // M.M
							set_DistMating( distance + model.calculDistanceEvol(eco, eco->rabbits[LocalFriends[cntrLocalFriends]].getFCM()));  // M.M modified
                            reprod(&eco->rabbits[LocalFriends[cntrLocalFriends]], newPrey, eco); //-- mePrey reproduces with rabbits[LocalFriends[k]]
                            pasFait = false;
                            setLastAction(FCMPrey::ReproduceAction);
                            //-- Meisam
                                model.ConceptIndex = FCMPrey::SearchPartner;
                                model.ConceptMultiplier = 0;
                                eco->rabbits[LocalFriends[cntrLocalFriends]].getFCM()->ConceptIndex = FCMPrey::SearchPartner;
                                eco->rabbits[LocalFriends[cntrLocalFriends]].getFCM()->ConceptMultiplier = 0;
                            //-- Meisam End
                        }
					}
					cntrLocalFriends += 1;
					//LocalFriends.erase(LocalFriends.begin() + k);
				} // -- End of while
			}

			LocalFriends.clear();

			if (pasFait){
				if (NBfriends > 0) {
					reproduceFailed2 /= (float) NBfriends;
					reproduceFailed3 /= (float) NBfriends;
					reproduceFailed4 /= (float) NBfriends;
					reproduceFailed5 /= (float) NBfriends;
					reproduceFailed6 /= (float) NBfriends;
				}
				setLastAction(FCMPrey::ReproduceFailAction); //-- Reproduction failed
			}
			break;
		}
		*/
		case FCMPrey::ReproduceAsexual: { //-- reproduce asexually		RS
			//-- Meisam, Feb 2011: 1) was rewrited. 2) Mating based on the all individuals in the current cell
			posNouvelle.x = getPosition()->x;
			posNouvelle.y = getPosition()->y;

			reproduceAsexualFailed1 = 0;				   //-- 1: Energy of prey is not enough

			bool pasFait = true;
			if ((this->getStateBirth() * this->getEnergyMax() * 1.9 * 1.3 * 1.05) / 100.0 > this->getEnergy()) { //We decided 16-18% per reproduction, so I'll call it 17 for now
				reproduceAsexualFailed1 = reproduceAsexualFailed1 | 1;  
				//printf("\nShouldBeAsexualRepFailure!");
				
			} //-- Energy of prey is not enough
			else{
				//printf("\nShouldBeAsexualReprod!\n");
				pasFait = false;
				set_DistMating(0.0);  // M.M modified //RS
				reprodAsexual(newPrey, eco); //RS
				setLastAction(FCMPrey::ReproduceAsexualAction);
				//-- Meisam
				model.ConceptIndex = FCMPrey::SearchPartner; // Need to figure out what to do with this?? //RS
				model.ConceptMultiplier = 0;
			}

			if (pasFait){
				setLastAction(FCMPrey::ReproduceAsexualFailAction); //-- Reproduction failed
			}
			break;
		}

		default:
			break;
	}//-- Endof switch
	
	float speedCurent = getPosition()->distance(posNouvelle, eco->getWidth(), eco->getHeight());
	setSpeed(speedCurent);
	return posNouvelle;
}

