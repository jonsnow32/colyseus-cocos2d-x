import {Planet} from "./planet"
import {Player} from "./player"
import {PLANET_STATE} from "./planet"

import * as shortid from "shortid";
export class World
{
    planets : {};
    currentMapID : 0;
    width : number;
    height : number;

    constructor(map)
    {
        this.currentMapID = 0;
        this.planets = {};
        var currentMap = map.layers[this.currentMapID++].objects;
        this.width = map.tilewidth*map.width;
        this.height = map.tileheight*map.height;
        for(var i = 0 ; i < currentMap.length ; i++)
        {
            if(currentMap[i].visible == true)
            {
                var plt = new Planet(currentMap[i].x + currentMap[i].width/2,this.height - currentMap[i].y - currentMap[i].height/2,currentMap[i].width/2,"",-1);
                if(currentMap[i].type == "PlayerInit")
                    plt.canInit = true;
                var planetID =  shortid.generate();
                this.planets[planetID] = plt;
                console.log(planetID);
            }
        }
    }
    findPlanetEmptyFor(clientID)
    {
        for(var key in this.planets)
        {
            if(this.planets[key].canInit && this.planets[key].playerID == "")
            {
                this.planets[key].playerID = clientID;
                this.planets[key].currentState  = PLANET_STATE.SPWAN;
                console.log("XYZ");
                break;
            }
        }
    }
    removeOwnerPlanet(clientID)
    {
        for(var key in  this.planets)
        {
            if(this.planets[key].playerID == clientID)
                this.planets[key].reset();
        }
    }
    depart(fromPlanetKeys,toPlanetKeys,nSatellite)
    {
        var src = this.planets[fromPlanetKeys];
        var des = this.planets[toPlanetKeys];
        var realnSatellite = src.depart(des);

        if(nSatellite > realnSatellite)
            console.log("detect cheat form " + this.planets[fromPlanetKeys].playerID);

    }
    attack(fromPlanetKeys,toPlanetKeys,nSatellite)
    {
        var src = this.planets[fromPlanetKeys];
        var des = this.planets[toPlanetKeys];
        des.onDamge(src,nSatellite);
    }
}