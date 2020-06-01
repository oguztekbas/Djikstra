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

cMessage* deneme = new cMessage(".");   // kodun cal�smas� icin cMessage a sag t�klay�p open declaration denmeli
                                        // ve SIM_API class�n�n alt�na public: simtime_t maliyet_bilgisi  property si eklenmeli !!!!!!!!!

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

               if(check_and_cast<cDelayChannel*>(this->gate("gate$o",i)->getChannel())->getDelay() < min_delay  ){  // DELAY i bu �ekilde alabiliyoruz.Kanal �zerinden.

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
      send(mesaj,"gate$o",this->gonderilecek_kapi->getIndex());    /// ROOT ���N D���k Delayli Channel � bulup o kanaldaki d���me g�nderiyoruz.




    }


}


void node::handleMessage(cMessage *msg)
{




        std::vector< cChannel* > gonderilebilecek_channellar;



            if(strcmp("node4",this->getFullName())== 0){



                    EV << this->getFullName();
                    EV << "   Toplam delay :" << msg->maliyet_bilgisi << "sn\n" ;
                    EV << "--HEDEF DUGUME GELINDI--";   // Hedef d�g�mse bitiriyoruz.



             }

          else{

 // d�g�mlerin gondermemesi ve buy�zden de kar��la�t�rma yapmamas� gereken kanallar� ��kartt�k.


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

              // Bu a�amalarda kap�lar�mdan kar��la�t�rma yapmayaca��m yani asla g�ndermice�im kap�lar� eliyoruz.
              // Ornek olarak node1 e geldi�inde sim�lasyon, node 1'in  root ve node 3  ile aras�ndaki kanalla ve kap�larla bi i�i yok.
              // o y�zden onlar� ��kar�p di�er kanallar�n delaylerini kar��la�t�r�p  d���k maliyetli olana g�nderim yap�yoruz a�a�� k�s�mlarda da.
              // yani bi agac yap�s�


                //TEST D�NG�S�.  for(int i = 0;  i < gonderilebilecek_channellar.size(); i++){

                //                 EV << check_and_cast<cDelayChannel*>(gonderilebilecek_channellar.at(i))->getDelay();

                //              }


                // bu k�s�m algoritmay� bar�nd�ran k�s�m  gonderibelec�im kanallar listesinde ilk elemandaki kanal� gonderece�im yermis gibi
                // belirliyoruz sonra listede ba�ka elemanlar varsa s�rayla bunlarla kar��la�t�r�yoruz
                // k�sacas� bi dizide en k�c�k say�y� bulma gibi bi�ey. burda maliyeti buluyoruz en k���k maliyetliye gidiyoruz.

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

                           msg->maliyet_bilgisi = min_delay;         // Bi �ncekiyle Toplanm�� halini koyuyoruz ki bi sonraki mant�kl� hesaplamalar yapabilsin.
                                                                     // djikstra da dugumler�n ag�rl�g� deg�s�yor sim�lasyon ilerledikce o mant�k.
                           send(msg,"gate$o",this->gonderilecek_kapi->getIndex());

            }








}
