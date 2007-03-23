
#include "EventFilter/EcalRawToDigiDev/interface/DCCTowerBlock.h"
#include "EventFilter/EcalRawToDigiDev/interface/DCCEventBlock.h"
#include "EventFilter/EcalRawToDigiDev/interface/DCCDataUnpacker.h"
#include "EventFilter/EcalRawToDigiDev/interface/DCCEventBlock.h"
#include "EventFilter/EcalRawToDigiDev/interface/ECALUnpackerException.h"
#include <stdio.h>
#include "EventFilter/EcalRawToDigiDev/interface/EcalElectronicsMapper.h"



DCCTowerBlock::DCCTowerBlock( DCCDataUnpacker * u, EcalElectronicsMapper * m, DCCEventBlock * e, bool unpack )
: DCCFEBlock(u,m,e,unpack){}


void DCCTowerBlock::updateCollectors(){

  DCCFEBlock::updateCollectors();
  
  // needs to be update for eb/ee
  digis_                  = unpacker_->ebDigisCollection();
   
  invalidGains_           = unpacker_->invalidGainsCollection();
  invalidGainsSwitch_     = unpacker_->invalidGainsSwitchCollection();
  invalidGainsSwitchStay_ = unpacker_->invalidGainsSwitchStayCollection();
  invalidChIds_           = unpacker_->invalidChIdsCollection();
 

}



void DCCTowerBlock::unpackXtalData(uint expStripID, uint expXtalID){
  
  bool errorOnXtal(false);
 
  uint16_t * xData_= reinterpret_cast<uint16_t *>(data_);

  // Get xtal data ids
  uint stripId = (*xData_) & TOWER_STRIPID_MASK;
  uint xtalId  =((*xData_)>>TOWER_XTALID_B ) & TOWER_XTALID_MASK;
  
  // cout<<"\n DEBUG : unpacked xtal data for strip id "<<stripId<<" and xtal id "<<xtalId<<endl;
  // cout<<"\n DEBUG : expected strip id "<<expStripID<<" expected xtal id "<<expXtalID<<endl;
  

  if( !zs_ && (expStripID != stripId || expXtalID != xtalId)){ 
	 
    LogWarning("EcalRawToDigi")<<"@SUB=DCCFEBlock::unpackXtalData"
      <<"\n For event "<<event_->l1A()<<",dcc "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
      <<"\n The expected strip is "<<expStripID<<" and "<<stripId<<" was found"
      <<"\n The expected xtal  is "<<expXtalID <<" and "<<xtalId<<" was found"<<endl;	

   
   pDetId_ = (EBDetId*) mapper_->getDetIdPointer(towerId_,expStripID,expXtalID);

   (*invalidChIds_)->push_back(*pDetId_);

    stripId = expStripID;
    xtalId  = expXtalID;
    errorOnXtal = true;
  
  }
  else if(zs_){
    // Check for valid Ids	 
    if(stripId == 0 || stripId > 5 || xtalId == 0 || xtalId > 5){
      cout<<"\n wrong xtal and strip id"<<endl; 
      LogWarning("EcalRawToDigi")<<"@SUB=DCCFEBlock::unpackXtalData"
        <<"\n For event "<<event_->l1A()<<",dcc "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
        <<"\n Unvalid strip : "<<stripId<<" or xtal : "<<xtalId<<" ids"<<endl;	
      //Todo : add to error collection
      errorOnXtal = true;
		
    }else{
	 
	 
      // check if strip and xtal id increases
      if ( stripId >= lastStripId_ ){
        if( stripId == lastStripId_ && xtalId < lastXtalId_ ){ 
		  
          LogWarning("EcalRawToDigi")<<"@SUB=DCCFEBlock::unpackXtalData"
            <<"\n For event "<<event_->l1A()<<",dcc "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
            <<"\n Xtal id was expected to increase but it didn't "
            <<"\n Last unpacked xtal was "<<lastXtalId_<<" while current xtal is "<<xtalId<<endl;
		   
           pDetId_ = (EBDetId*) mapper_->getDetIdPointer(towerId_,stripId,xtalId);


           (*invalidChIds_)->push_back(*pDetId_);
		  
	   errorOnXtal = true;
       }
      }
      else if( stripId < lastStripId_){
      
        LogWarning("EcalRawToDigi")<<"@SUB=DCCFEBlock::unpackXtalData"
          <<"\n For event "<<event_->l1A()<<",dcc "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
          <<"\n Strip id was expected to increase but it didn't "
          <<"\n Last unpacked strip was "<<lastStripId_<<" while current strip is "<<stripId<<endl;
 
       pDetId_ = (EBDetId*) mapper_->getDetIdPointer(towerId_,stripId,xtalId);
       (*invalidChIds_)->push_back(*pDetId_);
		
        errorOnXtal = true;		  
      }
		
      lastStripId_  = stripId;
      lastXtalId_   = xtalId;
    }
  }
 
 
  // if there is an error on xtal id ignore next error checks  
  if(!errorOnXtal){ 

     pDFId_ = (EBDataFrame*) mapper_->getDFramePointer(towerId_,stripId,xtalId);	
//     if(pDFId_){ //thisis not needed for the EB

      vector<int> xtalGain;   
      bool wrongGain(false);
	 
      //set samples in the frame
      for(uint i =0; i< nTSamples_ ;i++){ 
        xData_++;
        uint data =  (*xData_) & TOWER_DIGI_MASK;
        uint gain =  data>>12;
        xtalGain.push_back(gain);
        if(gain == 0){ wrongGain = true; } 
 
        pDFId_->setSample(i,data);
      }
	
    
      if(wrongGain){ 
        LogWarning("EcalRawToDigi")<<"@SUB=DCCFEBlock::unpackXtalData"
        <<"\n For event "<<event_->l1A()<<",dcc "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
        <<"\n A wrong gain was found in strip "<<stripId<<" and xtal "<<xtalId<<endl;   

        (*invalidGains_)->push_back(pDFId_->id());
        errorOnXtal = true;
      }
	
   
      short firstGainWrong=-1;
      short numGainWrong=0;
	    
      for (uint i=0; i<xtalGain.size(); i++ ) {
        if (i>0 && xtalGain[i-1]>xtalGain[i]) {
          numGainWrong++;
          if (firstGainWrong == -1) { firstGainWrong=i;}
        }
      }
   
      bool wrongGainStaysTheSame=false;
   
      if (firstGainWrong!=-1 && firstGainWrong<9){
        short gainWrong = xtalGain[firstGainWrong];
        // does wrong gain stay the same after the forbidden transition?
        for (unsigned short u=firstGainWrong+1; u<xtalGain.size(); u++){
          if( gainWrong == xtalGain[u]) wrongGainStaysTheSame=true; 
          else                          wrongGainStaysTheSame=false; 
        }// END loop on samples after forbidden transition
      }// if firstGainWrong!=0 && firstGainWrong<8

      if (numGainWrong>0) {

    
        LogWarning("EcalRawToDigi")<<"@SUB=DCCFEBlock::unpackXtalData"
          <<"\n For event "<<event_->l1A()<<",dcc "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
          <<"\n A wrong gain transition switch was found in strip "<<stripId<<" and xtal "<<xtalId<<endl;    

        (*invalidGainsSwitch_)->push_back(pDFId_->id());

         errorOnXtal = true;
      } 

      if(wrongGainStaysTheSame){

        LogWarning("EcalRawToDigi")<<"@SUB=DCCFEBlock::unpackXtalData"
          <<"\n For event "<<event_->l1A()<<",dcc "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
          <<"\n A wrong gain switch stay was found in strip "<<stripId<<" and xtal "<<xtalId<<endl;
      
       (*invalidGainsSwitchStay_)->push_back(pDFId_->id());       

        errorOnXtal = true;  
      }

      //Add frame to collection
      if(!errorOnXtal){ (*digis_)->push_back(*pDFId_);}

  
//   }// End on check of det id
  
  }  	
  
  //Point to begin of next xtal Block
  data_ += numbDWInXtalBlock_;
}





