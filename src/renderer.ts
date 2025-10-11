import { GameState, GameObjectType } from './types';
import { CANVAS_WIDTH, CANVAS_HEIGHT } from './gameLogic';

export const renderGame = (ctx: CanvasRenderingContext2D, state: GameState): void => {
  // Clear canvas
  ctx.clearRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);

  // Draw background (room)
  ctx.fillStyle = '#f5deb3'; // Wheat color for floor
  ctx.fillRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);

  // Draw walls
  ctx.strokeStyle = '#8b4513';
  ctx.lineWidth = 3;
  ctx.strokeRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);

  // Draw objects
  state.objects.forEach((obj) => {
    if (obj.isDestroyed) {
      // Draw destroyed object (debris)
      ctx.fillStyle = '#999';
      ctx.globalAlpha = 0.3;
      ctx.fillRect(obj.position.x, obj.position.y, obj.width, obj.height);
      ctx.globalAlpha = 1;
      
      // Draw X mark
      ctx.strokeStyle = '#ff0000';
      ctx.lineWidth = 2;
      ctx.beginPath();
      ctx.moveTo(obj.position.x, obj.position.y);
      ctx.lineTo(obj.position.x + obj.width, obj.position.y + obj.height);
      ctx.moveTo(obj.position.x + obj.width, obj.position.y);
      ctx.lineTo(obj.position.x, obj.position.y + obj.height);
      ctx.stroke();
    } else {
      // Draw intact object
      switch (obj.type) {
        case GameObjectType.VASE:
          // Draw vase
          ctx.fillStyle = '#4169e1'; // Royal blue
          ctx.beginPath();
          ctx.ellipse(
            obj.position.x + obj.width / 2,
            obj.position.y + obj.height - 10,
            obj.width / 2,
            10,
            0,
            0,
            2 * Math.PI
          );
          ctx.fill();
          ctx.fillRect(
            obj.position.x + obj.width / 4,
            obj.position.y,
            obj.width / 2,
            obj.height - 10
          );
          break;
        case GameObjectType.FURNITURE:
          // Draw furniture (table)
          ctx.fillStyle = '#8b4513'; // Saddle brown
          ctx.fillRect(obj.position.x, obj.position.y, obj.width, obj.height);
          ctx.fillStyle = '#654321';
          ctx.fillRect(obj.position.x + 5, obj.position.y + 5, obj.width - 10, obj.height - 10);
          break;
        case GameObjectType.CURTAIN:
          // Draw curtain
          ctx.fillStyle = '#dc143c'; // Crimson
          for (let i = 0; i < 5; i++) {
            ctx.fillRect(
              obj.position.x + (i * obj.width) / 5,
              obj.position.y,
              obj.width / 6,
              obj.height
            );
          }
          break;
        case GameObjectType.FOOD:
          // Draw food (fish)
          ctx.fillStyle = '#ff69b4'; // Hot pink
          ctx.beginPath();
          ctx.ellipse(
            obj.position.x + obj.width / 2,
            obj.position.y + obj.height / 2,
            obj.width / 2,
            obj.height / 3,
            0,
            0,
            2 * Math.PI
          );
          ctx.fill();
          // Triangle tail
          ctx.beginPath();
          ctx.moveTo(obj.position.x + obj.width, obj.position.y + obj.height / 2);
          ctx.lineTo(obj.position.x + obj.width + 5, obj.position.y + obj.height / 2 - 5);
          ctx.lineTo(obj.position.x + obj.width + 5, obj.position.y + obj.height / 2 + 5);
          ctx.closePath();
          ctx.fill();
          break;
      }
    }
  });

  // Draw humans
  state.humans.forEach((human) => {
    // Draw body
    ctx.fillStyle = '#2f4f4f'; // Dark slate gray
    ctx.fillRect(human.position.x, human.position.y, human.width, human.height);

    // Draw head
    ctx.fillStyle = '#ffdbac'; // Peach
    ctx.beginPath();
    ctx.arc(
      human.position.x + human.width / 2,
      human.position.y - 10,
      15,
      0,
      2 * Math.PI
    );
    ctx.fill();

    // Draw detection cone (debug visualization)
    ctx.strokeStyle = 'rgba(255, 0, 0, 0.2)';
    ctx.fillStyle = 'rgba(255, 0, 0, 0.1)';
    ctx.beginPath();
    ctx.moveTo(human.position.x + human.width / 2, human.position.y + human.height / 2);
    ctx.arc(
      human.position.x + human.width / 2,
      human.position.y + human.height / 2,
      human.detectionRadius,
      human.lookDirection - Math.PI / 3,
      human.lookDirection + Math.PI / 3
    );
    ctx.closePath();
    ctx.fill();
    ctx.stroke();

    // Draw look direction indicator
    ctx.strokeStyle = '#ff0000';
    ctx.lineWidth = 3;
    ctx.beginPath();
    ctx.moveTo(human.position.x + human.width / 2, human.position.y + human.height / 2);
    ctx.lineTo(
      human.position.x + human.width / 2 + Math.cos(human.lookDirection) * 40,
      human.position.y + human.height / 2 + Math.sin(human.lookDirection) * 40
    );
    ctx.stroke();
  });

  // Draw cat
  const cat = state.cat;
  
  // Cat body
  ctx.fillStyle = state.cat.isSprinting ? '#ff8c00' : '#ff6347'; // Orange when sprinting, tomato red normal
  ctx.beginPath();
  ctx.ellipse(
    cat.position.x + cat.width / 2,
    cat.position.y + cat.height / 2,
    cat.width / 2,
    cat.height / 2.5,
    0,
    0,
    2 * Math.PI
  );
  ctx.fill();

  // Cat head
  ctx.beginPath();
  ctx.arc(cat.position.x + cat.width / 2, cat.position.y + cat.height / 3, 12, 0, 2 * Math.PI);
  ctx.fill();

  // Cat ears
  ctx.fillStyle = state.cat.isSprinting ? '#ff8c00' : '#ff6347';
  ctx.beginPath();
  ctx.moveTo(cat.position.x + cat.width / 2 - 8, cat.position.y + cat.height / 3 - 8);
  ctx.lineTo(cat.position.x + cat.width / 2 - 12, cat.position.y + cat.height / 3 - 18);
  ctx.lineTo(cat.position.x + cat.width / 2 - 4, cat.position.y + cat.height / 3 - 12);
  ctx.fill();

  ctx.beginPath();
  ctx.moveTo(cat.position.x + cat.width / 2 + 8, cat.position.y + cat.height / 3 - 8);
  ctx.lineTo(cat.position.x + cat.width / 2 + 12, cat.position.y + cat.height / 3 - 18);
  ctx.lineTo(cat.position.x + cat.width / 2 + 4, cat.position.y + cat.height / 3 - 12);
  ctx.fill();

  // Cat eyes
  ctx.fillStyle = '#000';
  ctx.beginPath();
  ctx.arc(cat.position.x + cat.width / 2 - 4, cat.position.y + cat.height / 3, 2, 0, 2 * Math.PI);
  ctx.fill();
  ctx.beginPath();
  ctx.arc(cat.position.x + cat.width / 2 + 4, cat.position.y + cat.height / 3, 2, 0, 2 * Math.PI);
  ctx.fill();

  // Cat tail
  ctx.strokeStyle = state.cat.isSprinting ? '#ff8c00' : '#ff6347';
  ctx.lineWidth = 4;
  ctx.beginPath();
  ctx.moveTo(cat.position.x + cat.width, cat.position.y + cat.height / 2);
  ctx.quadraticCurveTo(
    cat.position.x + cat.width + 10,
    cat.position.y + cat.height / 2 - 15,
    cat.position.x + cat.width + 5,
    cat.position.y + cat.height / 2 - 25
  );
  ctx.stroke();
};
