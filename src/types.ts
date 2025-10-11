// Game types and interfaces

export interface Position {
  x: number;
  y: number;
}

export interface Velocity {
  dx: number;
  dy: number;
}

export interface GameObject {
  id: string;
  position: Position;
  width: number;
  height: number;
  type: GameObjectType;
  isDestroyed: boolean;
  points: number;
}

export enum GameObjectType {
  VASE = 'vase',
  FURNITURE = 'furniture',
  CURTAIN = 'curtain',
  FOOD = 'food',
}

export interface Cat {
  position: Position;
  velocity: Velocity;
  width: number;
  height: number;
  isSprinting: boolean;
}

export interface Human {
  position: Position;
  width: number;
  height: number;
  lookDirection: number; // angle in radians
  detectionRadius: number;
}

export interface GameState {
  cat: Cat;
  objects: GameObject[];
  humans: Human[];
  score: number;
  caught: boolean;
  gameOver: boolean;
}
