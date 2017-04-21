export declare class World {
    planets: {};
    currentMapID: 0;
    width: number;
    height: number;
    constructor(map: any);
    findPlanetEmptyFor(clientID: any): void;
    removeOwnerPlanet(clientID: any): void;
    depart(fromPlanetKeys: any, toPlanetKeys: any, nSatellite: any): void;
    attack(fromPlanetKeys: any, toPlanetKeys: any, nSatellite: any): void;
}
