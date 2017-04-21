import { World } from "./world";
export declare class State {
    world: World;
    score: 0;
    players: {};
    ownerID: string;
    team: {};
    constructor(world: any);
    addPlayer(clientID: any): void;
    getColor(color: any): 0 | 1 | 2 | 3 | 4;
    removePlayer(clientID: any): void;
}
