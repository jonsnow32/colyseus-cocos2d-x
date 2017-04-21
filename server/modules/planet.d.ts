export declare enum PLANET_STATE {
    FREE = 0,
    BUILDING = 1,
    SPWAN = 2,
}
export declare class Planet {
    maxSatellite: number;
    spwanSpeed: number;
    team: number;
    playerID: string;
    id: number;
    x: number;
    y: number;
    canInit: boolean;
    nSatellite: number;
    currentState: PLANET_STATE;
    constructor(x: any, y: any, maxSatellite: any, player: any, team: any);
    reset(): void;
    upgrade(): void;
    depart(toPlanet: any): number;
    onDamge(fromPlanet: any, nSatellite: any): void;
    getCurrentState(): PLANET_STATE;
}
