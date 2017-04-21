import {Player} from "./player"

export enum PLANET_STATE {
      FREE = 0,
      BUILDING,
      SPWAN,
};

export class Planet
{
    maxSatellite : number;
    spwanSpeed : number;
    team : number;
    playerID : string;
    id : number;
    x : number ;
    y : number;
    canInit : boolean;
    nSatellite : number;
    currentState : PLANET_STATE;
    constructor(x,y,maxSatellite,player,team)
    {
        this.x = x;
        this.y = y;
        this.team = team;
        this.playerID = player;
        this.maxSatellite = maxSatellite;
        this.spwanSpeed = 10.0/maxSatellite;
        this.canInit = false;
        this.nSatellite = 1;
        this.currentState = PLANET_STATE.FREE;

        setInterval(() => {
            if(this.currentState == PLANET_STATE.SPWAN && this.nSatellite < this.maxSatellite)
            {
                this.nSatellite++;
                console.log("nSatellite= " + this.nSatellite);
            }
        }, this.spwanSpeed*1000)

    }
    reset()
    {
        this.playerID = "";
        this.nSatellite = 1;
    }
    upgrade()
    {
        
    }
    depart(toPlanet)
    {
        var temp = this.nSatellite;
        this.nSatellite = 1;
        return temp;
    }
    onDamge(fromPlanet,nSatellite)
    {
        if(this.currentState == PLANET_STATE.FREE)
        {
            this.nSatellite += nSatellite;
            this.playerID = fromPlanet.playerID;
            this.currentState = PLANET_STATE.BUILDING;
        }
        else if(this.currentState == PLANET_STATE.BUILDING)
        {
            if(this.playerID == fromPlanet.playerID)
                this.nSatellite += nSatellite;
            else
                this.nSatellite -= nSatellite;

            if(this.nSatellite <= 0)
                this.playerID = fromPlanet.playerID;
            
            this.nSatellite = Math.abs(this.nSatellite);

            if(this.nSatellite >= this.maxSatellite)
            {
                this.currentState = PLANET_STATE.SPWAN;
            }   
        }
        else if(this.currentState == PLANET_STATE.SPWAN)
        {
            if(this.playerID == fromPlanet.playerID)
                this.nSatellite += nSatellite;
            else
                this.nSatellite -= nSatellite;

            if(this.nSatellite <= 0)
                this.playerID = fromPlanet.playerID;
                
            this.nSatellite = Math.abs(this.nSatellite);  
        }
    }
    getCurrentState()
    {
        return this.currentState;
    }
}