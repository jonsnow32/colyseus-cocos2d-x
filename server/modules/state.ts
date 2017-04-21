import {Player} from "./player"
import {World} from "./world"

export class State
{
    world : World;
    score : 0;
    players: {};
    ownerID : string;
    team : {};
    constructor(world)
    {
        this.world = world;
        this.players = {};
        this.team = {"red":"","blue":"","green":"","yello":""};
    }
    addPlayer(clientID)
    {
        for(var key in this.team)
        {
            if(this.team[key] == "")
            {
                this.team[key] = clientID;
                var player = new Player(clientID,this.getColor(key));
                this.players[clientID] = player;
                this.world.findPlanetEmptyFor(clientID);
                break;
            }
        }
        
    }
    getColor(color)
    {
        if(color == "red")
            return 1;
        if(color == "blue")
            return 2;
        if(color == "green")
            return 3;
        if(color == "yellow")
            return 4;
        return 0;
    }
    removePlayer(clientID)
    {
        delete this.players[clientID];
        for(var key in this.team)
        {
            if(this.team[key] == clientID)
                this.team[key] = "";
        }
        this.world.removeOwnerPlanet(clientID);
    }
}
