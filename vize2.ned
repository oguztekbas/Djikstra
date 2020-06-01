simple node
{
    gates:
        inout gate[];
}

network Network
{
    @display("bgb=632,406");
    
   
        
    
    submodules:
        root: node {
            @display("p=73,121");
        }
        node1: node {
            @display("p=212,49");
        }
        node2: node {
            @display("p=408,49");
        }
        node3: node {
            @display("p=212,190");
        }
        node4: node {
            @display("p=408,190");
        }
    connections:

 																   // -------------------------------------------------
                                                                  
        root.gate++ <--> { delay = 8s;}  <--> node1.gate++;        // Verilen ödevin simülasyonundaki delayler girildi. 
        node1.gate++ <-->  { delay = 1s;} <--> node2.gate++;       // .cc dosyasýnda da  bu kanallardaki delayler channel nesneleri aracýlýðýyla
        node4.gate++ <-->  { delay = 6s;}  <--> node3.gate++;      // cekildi ve karþýlaþtýrma yapýldý.
        node3.gate++ <--> { delay = 2s;}  <--> root.gate++;        // ona göre de gidilecek yol her aþamada belirlendi.
        node1.gate++ <-->  { delay = 2s;} <--> node4.gate++;       // Bu simülasyonda Root kaynak dügüm , node4 target-hedef dügüm.
        node2.gate++ <-->{ delay = 4s;}  <--> node4.gate++;
        node1.gate++ <-->{ delay = 1s;} <--> node3.gate++;         // -------------------------------------------------
}