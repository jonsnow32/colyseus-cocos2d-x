

import { Room } from "colyseus";
import { State } from "../modules/state";
import { World } from "../modules/world";
import { Player } from "../modules/player";

import * as maps from "../maps/map.json";
import * as msgpack from "msgpack-lite";

enum Command {
      DEPART = 1,
      ATTACK,
      SEND_ATTACK_TO_ONOTHER,
};

export class GameRoom extends Room<any> {

 constructor ( options ) {
    super( options );
    //this.setPatchRate( 1000 );
    var world = new World(maps);

    this.setState(new State(world));
    console.log("GameRoom created!", options);

    console.log("maps" + JSON.stringify(maps));
  }

  requestJoin (options) {
    return ( this.clients.length < 5 )
  }

  onJoin (client) {
    this.state.addPlayer(client.id);
  }

  onLeave (client) {
    this.state.removePlayer(client.id);
  }

  onMessage (client, message) {
    console.log(message, " GameRoom received from", client.id);
    //this.state.onMessage(client.id,data);

    //attack to clientID //param //from/to/nSatellite
    var command = message[0];
    var data = message[1];

    if(command == Command.DEPART)
    {
        this.state.world.depart(message[1],message[2],message[3]);
        this.sendAllExcept(client.id,[Command.SEND_ATTACK_TO_ONOTHER,message[1],message[2],message[3]]);
    }
    //depart params : fromPlanet ToPlanet mSatellite 
    if(command == Command.ATTACK)
    {
      this.state.world.attack(message[1],message[2],message[3]);
    }
                                         
  }
  sendAllExcept(clientID,data)
  {
      console.log("sendAllExcept clientID :" + clientID);
      for(var i = 0 ; i < this.clients.length ; i++)
      {
          console.log("sendAllExcept :" + this.clients[i].id);
          if(this.clients[i].id != clientID)
          {
              
              this.send(this.clients[i],data);
          }
      }
  }
  update () {
    this.state.update();
  }
  onDispose () {
    console.log("Dispose GameRoom");
  }

}
