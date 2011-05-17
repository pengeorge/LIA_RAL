/*
This file is part of LIA_RAL which is a set of software based on ALIZE
toolkit for speaker recognition. ALIZE toolkit is required to use LIA_RAL.

LIA_RAL project is a development project was initiated by the computer
science laboratory of Avignon / France (Laboratoire Informatique d'Avignon -
LIA) [http://lia.univ-avignon.fr <http://lia.univ-avignon.fr/>]. Then it
was supported by two national projects of the French Research Ministry:
	- TECHNOLANGUE program [http://www.technolangue.net]
	- MISTRAL program [http://mistral.univ-avignon.fr]

LIA_RAL is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or any later version.

LIA_RAL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with LIA_RAL.
If not, see [http://www.gnu.org/licenses/].

The LIA team as well as the LIA_RAL project team wants to highlight the
limits of voice authentication in a forensic context.
The "Person Authentification by Voice: A Need of Caution" paper
proposes a good overview of this point (cf. "Person
Authentification by Voice: A Need of Caution", Bonastre J.F.,
Bimbot F., Boe L.J., Campbell J.P., Douglas D.A., Magrin-
chagnolleau I., Eurospeech 2003, Genova].
The conclusion of the paper of the paper is proposed bellow:
[Currently, it is not possible to completely determine whether the
similarity between two recordings is due to the speaker or to other
factors, especially when: (a) the speaker does not cooperate, (b) there
is no control over recording equipment, (c) recording conditions are not
known, (d) one does not know whether the voice was disguised and, to a
lesser extent, (e) the linguistic content of the message is not
controlled. Caution and judgment must be exercised when applying speaker
recognition techniques, whether human or automatic, to account for these
uncontrolled factors. Under more constrained or calibrated situations,
or as an aid for investigative purposes, judicious application of these
techniques may be suitable, provided they are not considered as infallible.
At the present time, there is no scientific process that enables one to
uniquely characterize a persones voice or to identify with absolute
certainty an individual from his or her voice.]

Copyright (C) 2004-2010
Laboratoire d'informatique d'Avignon [http://lia.univ-avignon.fr]
LIA_RAL admin [alize@univ-avignon.fr]
Jean-Francois Bonastre [jean-francois.bonastre@univ-avignon.fr]
*/

#include <iostream>
#include <liatools.h>
#include <EigenChannel.h>

using namespace alize;
using namespace std;

int main(int argc, char* argv[]) {
	ConfigChecker cc;
	try {
		// Needed params
		cc.addStringParam("ndxFilename",true,true,"NDX of multiple GMM speaker recordings");
		cc.addStringParam("inputWorldFilename",true,true,"the world model file");
		cc.addIntegerParam("nbIt",true,true,"number of ml it");	
		cc.addStringParam("saveMatrixFormat",true,true,"matrix format: DB (binary) or DT (ascii)");		  
		cc.addStringParam("loadMatrixFormat",true,true,"matrix format: DB (binary) or DT (ascii)");				
		// Optionnal
		cc.addBooleanParam("loadAccs",false,true,"if true do not compute UBM stats, load matrices");		
		cc.addIntegerParam("computeLLK",false,true,"optional: nb of files where LLK is computed");	
		cc.addStringParam("initChannelMatrix",false,true,"init Channel Matrix (DM compatibility)");	
		cc.addIntegerParam("channelMatrixRank",false,true,"final rank of channel matrix (DM compatibility)");	
		cc.addFloatParam("regulationFactor",false,true,"map tau (DM compatibility)");
		cc.addStringParam("initEigenChannelMatrix",false,true,"init EigenChannel Matrix if not loaded");
		cc.addStringParam("loadInitChannelMatrix",false,true,"filename to load init Matrix ");	
		cc.addStringParam("eigenChannelMatrix",false,true,"filename to save EigenVoice Matrix ");					
		cc.addIntegerParam("eigenChannelRank",false,true,"final rank of EigenVoice matrix");
		cc.addStringParam("eigenChannelMode",false,true,"compute eigenChannel matrix for JFA or LFA paradigm WARNING if not set call DM version");
		cc.addStringParam("channelMatrix",false,true,"filename to save Channel Matrix ");					
		cc.addStringParam("eigenVoiceMatrix",false,true,"name of the EigenVoice Matrix");
		cc.addBooleanParam("checkLLK",false,true,"if true do compute the likelihood of training data after each iteration");
		cc.addBooleanParam("saveInitChannelMatrix",false,true,"if true save the matrix used for initialisation");
		cc.addBooleanParam("saveAllECMatrices",false,true,"if true save the matrices after each iteration");

		// Insertion of config compatibility rules
		CmdLine cmdLine(argc, argv);
	     if (cmdLine.displayHelpRequired()){
			cout << "****************************************" << endl;
			cout << "********** EigenChannel.exe ************" << endl;
			cout << "****************************************" << endl;
			cout << endl;
			cout << "Evaluate Channel Matrix from speakers data" << endl;
	  	   cout <<endl<<cc.getParamList()<<endl;
       	 return 0;  
	      }
      	if (cmdLine.displayVersionRequired()){
      	  cout <<"Version 2.0 ALIZE Package"<<endl;
      	} 

		Config tmp;
		cmdLine.copyIntoConfig (tmp);
		Config config;
		if (tmp.existsParam("config")) config.load(tmp.getParam("config"));
		cmdLine.copyIntoConfig(config);
		cc.check(config);
		debug=config.getParam_debug();	
		if (config.existsParam("verbose"))verbose=config.getParam("verbose").toBool();else verbose=false;
		if (verbose) verboseLevel=1;else verboseLevel=0;
		if (config.existsParam("verboseLevel"))verboseLevel=config.getParam("verboseLevel").toLong();
		if (verboseLevel>0) verbose=true;		
		if (cmdLine.displayHelpRequired()) {cout << cc.getParamList() << endl;}	

		if(config.getParam("eigenChannelMode") == "JFA")		EigenChannelJFA(config);
		else if(config.getParam("eigenChannelMode") == "LFA")	EigenChannelLFA(config);
		else if(config.getParam("eigenChannelMode") == "")	EigenChannel(config);
		else{
			cout<<"Error : wrong eigenChannelMode parameter, please chose JFA or LFA"<<endl;
		}
	}
	catch (Exception& e) {cout << e.toString() << cc.getParamList() << endl;}
if (debug) {
}
return 0;
}
