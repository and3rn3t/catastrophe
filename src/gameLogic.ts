import { GameState, GameObjectType, GameObject, Position } from './types';

export const CANVAS_WIDTH = 800;
export const CANVAS_HEIGHT = 600;
export const CAT_SIZE = 30;
export const HUMAN_SIZE = 40;
export const CAT_SPEED = 3;
export const CAT_SPRINT_SPEED = 5;
export const INTERACTION_RADIUS = 50;

export const createInitialState = (): GameState => {
  return {
    cat: {
      position: { x: 100, y: 100 },
      velocity: { dx: 0, dy: 0 },
      width: CAT_SIZE,
      height: CAT_SIZE,
      isSprinting: false,
    },
    objects: createInitialObjects(),
    humans: [
      {
        position: { x: 600, y: 400 },
        width: HUMAN_SIZE,
        height: HUMAN_SIZE,
        lookDirection: Math.PI,
        detectionRadius: 150,
      },
      {
        position: { x: 400, y: 200 },
        width: HUMAN_SIZE,
        height: HUMAN_SIZE,
        lookDirection: 0,
        detectionRadius: 150,
      },
    ],
    score: 0,
    caught: false,
    gameOver: false,
  };
};

const createInitialObjects = (): GameObject[] => {
  return [
    // Vases
    {
      id: 'vase1',
      position: { x: 200, y: 150 },
      width: 30,
      height: 40,
      type: GameObjectType.VASE,
      isDestroyed: false,
      points: 10,
    },
    {
      id: 'vase2',
      position: { x: 600, y: 150 },
      width: 30,
      height: 40,
      type: GameObjectType.VASE,
      isDestroyed: false,
      points: 10,
    },
    // Furniture
    {
      id: 'table1',
      position: { x: 350, y: 300 },
      width: 100,
      height: 60,
      type: GameObjectType.FURNITURE,
      isDestroyed: false,
      points: 15,
    },
    // Curtains
    {
      id: 'curtain1',
      position: { x: 50, y: 50 },
      width: 40,
      height: 80,
      type: GameObjectType.CURTAIN,
      isDestroyed: false,
      points: 20,
    },
    {
      id: 'curtain2',
      position: { x: 700, y: 50 },
      width: 40,
      height: 80,
      type: GameObjectType.CURTAIN,
      isDestroyed: false,
      points: 20,
    },
    // Food
    {
      id: 'food1',
      position: { x: 300, y: 500 },
      width: 25,
      height: 25,
      type: GameObjectType.FOOD,
      isDestroyed: false,
      points: 25,
    },
    {
      id: 'food2',
      position: { x: 500, y: 500 },
      width: 25,
      height: 25,
      type: GameObjectType.FOOD,
      isDestroyed: false,
      points: 25,
    },
  ];
};

export const checkCollision = (
  pos1: Position,
  size1: { width: number; height: number },
  pos2: Position,
  size2: { width: number; height: number }
): boolean => {
  return (
    pos1.x < pos2.x + size2.width &&
    pos1.x + size1.width > pos2.x &&
    pos1.y < pos2.y + size2.height &&
    pos1.y + size1.height > pos2.y
  );
};

export const getDistance = (pos1: Position, pos2: Position): number => {
  const dx = pos1.x - pos2.x;
  const dy = pos1.y - pos2.y;
  return Math.sqrt(dx * dx + dy * dy);
};

export const isInDetectionCone = (
  humanPos: Position,
  humanDir: number,
  catPos: Position,
  radius: number
): boolean => {
  const dx = catPos.x - humanPos.x;
  const dy = catPos.y - humanPos.y;
  const distance = Math.sqrt(dx * dx + dy * dy);

  if (distance > radius) return false;

  const angleToTarget = Math.atan2(dy, dx);
  const angleDiff = Math.abs(((angleToTarget - humanDir + Math.PI) % (2 * Math.PI)) - Math.PI);

  // Human can see in a 90-degree cone
  return angleDiff < Math.PI / 3;
};
