#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;
std::vector< cChannel* > visited_channels;




class node : public cSimpleModule
{

  public: cGate* gonderilecek_kapi;


  protected:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};


Define_Module(node);

void node::initialize()
{

cMessage* deneme = new cMessage(".");   // kodun calýsmasý icin cMessage a sag týklayýp open declaration denmeli
                                        // ve SIM_API classýnýn altýna public: simtime_t maliyet_bilgisi  property si eklenmeli !!!!!!!!!

    if(strcmp("root",this->getName())== 0){

        for(int i=0; i < visited_channels.size(); i++){

               visited_channels.pop_back();

       }



        cDelayChannel * channel1 = check_and_cast<cDelayChannel*>(this->gate("gate$o",0)->getChannel());


     //CONSOL TEST   // EV << this->gate("gate$o", 1)->getIndex();
     //CONSOL TEST   //EV << this->gate("gate$o", 1)->getChannel()->getSourceGate()->getIndex();


        simtime_t min_delay = channel1->getDelay();
        this->gonderilecek_kapi = this->gate("gate$o", 0);


        for(int i = 1; i < this->gateCount()/2; i++){

               if(check_and_cast<cDelayChannel*>(this->gate("gate$o",i)->getChannel())->getDelay() < min_delay  ){  // DELAY i bu þekilde alabiliyoruz.Kanal üzerinden.

                   min_delay =  check_and_cast<cDelayChannel*>(this->gate("gate$o",i)->getChannel())->getDelay();
                   this->gonderilecek_kapi = this->gate("gate$o", i);

               }

        }



        for(int i = 0; i < this->gateCount()/2;  i++ ){

                visited_channels.push_back(this->gate("gate$o", i)->getChannel());

        }

      cMessage* mesaj = new cMessage("selam",2);
      mesaj->maliyet_bilgisi = min_delay;

      EV << this->getFullName() << "   ";
      EV << "Secilen delay :" << mesaj->maliyet_bilgisi << "sn" ;
      send(mesaj,"gate$o",this->gonderilecek_kapi->getIndex());    /// ROOT ÝÇÝN Düþük Delayli Channel ý bulup o kanaldaki düðüme gönderiyoruz.




    }


}


void node::handleMessage(cMessage *msg)
{




        std::vector< cChannel* > gonderilebilecek_channellar;



            if(strcmp("node4",this->getFullName())== 0){



                    EV << this->getFullName();
                    EV << "   Toplam delay :" << msg->maliyet_bilgisi << "sn\n" ;
                    EV << "--HEDEF DUGUME GELINDI--";   // Hedef dügümse bitiriyoruz.



             }

          else{

 // dügümlerin gondermemesi ve buyüzden de karþýlaþtýrma yapmamasý gereken kanallarý çýkarttýk.


                for(int i = 0; i < this->gateCount()/2; i++){


                    if(msg->getArrivalGate()->getIndex() == this->gate("gate$o", i)->getIndex()){

                                          continue;
                    }

                   gonderilebilecek_channellar.push_back(this->gate("gate$o", i)->getChannel());

                }


                for(int i = 0; i < visited_channels.size(); i++){

                     for(int j = 0; j < gonderilebilecek_channellar.size(); j++){

                         if(gonderilebilecek_channellar.at(j)->getId() == visited_channels.at(i)->getId()){

                             gonderilebilecek_channellar.erase(gonderilebilecek_channellar.begin() + j);
                         }
                     }
                }

              // Bu aþamalarda kapýlarýmdan karþýlaþtýrma yapmayacaðým yani asla göndermiceðim kapýlarý eliyoruz.
              // Ornek olarak node1 e geldiðinde simülasyon, node 1'in  root ve node 3  ile arasýndaki kanalla ve kapýlarla bi iþi yok.
              // o yüzden onlarý çýkarýp diðer kanallarýn delaylerini karþýlaþtýrýp  düþük maliyetli olana gönderim yapýyoruz aþaðý kýsýmlarda da.
              // yani bi agac yapýsý


                //TEST DÖNGÜSÜ.  for(int i = 0;  i < gonderilebilecek_channellar.size(); i++){

                //                 EV << check_and_cast<cDelayChannel*>(gonderilebilecek_channellar.at(i))->getDelay();

                //              }


                // bu kýsým algoritmayý barýndýran kýsým  gonderibelecðim kanallar listesinde ilk elemandaki kanalý gondereceðim yermis gibi
                // belirliyoruz sonra listede baþka elemanlar varsa sýrayla bunlarla karþýlaþtýrýyoruz
                // kýsacasý bi dizide en kücük sayýyý bulma gibi biþey. burda maliyeti buluyoruz en küçük maliyetliye gidiyoruz.

                simtime_t min_delay = check_and_cast<cDelayChannel*>(gonderilebilecek_channellar.at(0))->getDelay() + msg->maliyet_bilgisi;
                this->gonderilecek_kapi = gonderilebilecek_channellar.at(0)->getSourceGate();

                    //CONSOL TEST        EV << "min_delay" << min_delay;
                    //CONSOL TEST        EV << "kapi" << this->gonderilecek_kapi->getIndex();
                    //CONSOL TEST        EV << "Size" << gonderilebilecek_channellar.size();

                        for(int i = 1; i < gonderilebilecek_channellar.size(); i++){

                               if(check_and_cast<cDelayChannel*>(gonderilebilecek_channellar.at(i))->getDelay() + msg->maliyet_bilgisi < min_delay  ){

                                   min_delay =  check_and_cast<cDelayChannel*>(gonderilebilecek_channellar.at(i))->getDelay() + msg->maliyet_bilgisi;
                                   this->gonderilecek_kapi = gonderilebilecek_channellar.at(i)->getSourceGate();


                               }

                        }




                        //CONSOL TEST          EV << "yeni min_delay" << min_delay;
                        //CONSOL TEST         EV << "yeni kapi" << this->gonderilecek_kapi->getIndex();


               for(int i = 0; i < gonderilebilecek_channellar.size();  i++ ){

                     visited_channels.push_back(gonderilebilecek_channellar.at(i));

               }
                     EV << this->getFullName() << "   ";
                     EV << "Secilen delay :" << msg->maliyet_bilgisi << "sn" ;

                           msg->maliyet_bilgisi = min_delay;         // Bi öncekiyle Toplanmýþ halini koyuyoruz ki bi sonraki mantýklý hesaplamalar yapabilsin.
                                                                     // djikstra da dugumlerýn agýrlýgý degýsýyor simülasyon ilerledikce o mantýk.
                           send(msg,"gate$o",this->gonderilecek_kapi->getIndex());

            }








}
