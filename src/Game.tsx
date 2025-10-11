import React, { useEffect, useRef, useState, useCallback } from 'react';
import { GameState } from './types';
import {
  createInitialState,
  CANVAS_WIDTH,
  CANVAS_HEIGHT,
  CAT_SPEED,
  CAT_SPRINT_SPEED,
  INTERACTION_RADIUS,
  getDistance,
  isInDetectionCone,
} from './gameLogic';
import { renderGame } from './renderer';
import './Game.css';

const Game: React.FC = () => {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [gameState, setGameState] = useState<GameState>(createInitialState());
  const [keys, setKeys] = useState<Set<string>>(new Set());
  const animationFrameRef = useRef<number | null>(null);

  // Handle keyboard input
  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      setKeys((prev) => new Set(prev).add(e.key.toLowerCase()));
    };

    const handleKeyUp = (e: KeyboardEvent) => {
      setKeys((prev) => {
        const newSet = new Set(prev);
        newSet.delete(e.key.toLowerCase());
        return newSet;
      });
    };

    window.addEventListener('keydown', handleKeyDown);
    window.addEventListener('keyup', handleKeyUp);

    return () => {
      window.removeEventListener('keydown', handleKeyDown);
      window.removeEventListener('keyup', handleKeyUp);
    };
  }, []);

  // Handle interactions
  const handleInteraction = useCallback((state: GameState): GameState => {
    const newObjects = state.objects.map((obj) => {
      if (obj.isDestroyed) return obj;

      const distance = getDistance(state.cat.position, obj.position);
      if (distance < INTERACTION_RADIUS) {
        return { ...obj, isDestroyed: true };
      }
      return obj;
    });

    const pointsGained = newObjects.reduce((acc, obj, idx) => {
      if (obj.isDestroyed && !state.objects[idx].isDestroyed) {
        return acc + obj.points;
      }
      return acc;
    }, 0);

    return {
      ...state,
      objects: newObjects,
      score: state.score + pointsGained,
    };
  }, []);

  // Check if cat is detected by humans
  const checkDetection = useCallback((state: GameState): boolean => {
    return state.humans.some((human) =>
      isInDetectionCone(
        human.position,
        human.lookDirection,
        state.cat.position,
        human.detectionRadius
      )
    );
  }, []);

  // Game loop
  useEffect(() => {
    if (gameState.gameOver) return;

    const gameLoop = () => {
      setGameState((prevState) => {
        let newState = { ...prevState };

        // Update cat velocity based on input
        let dx = 0;
        let dy = 0;

        if (keys.has('arrowup') || keys.has('w')) dy -= 1;
        if (keys.has('arrowdown') || keys.has('s')) dy += 1;
        if (keys.has('arrowleft') || keys.has('a')) dx -= 1;
        if (keys.has('arrowright') || keys.has('d')) dx += 1;

        const isSprinting = keys.has('shift');
        const speed = isSprinting ? CAT_SPRINT_SPEED : CAT_SPEED;

        // Normalize diagonal movement
        if (dx !== 0 && dy !== 0) {
          dx *= 0.707;
          dy *= 0.707;
        }

        // Update cat position
        const newCat = {
          ...newState.cat,
          velocity: { dx: dx * speed, dy: dy * speed },
          isSprinting,
          position: {
            x: Math.max(
              0,
              Math.min(
                CANVAS_WIDTH - newState.cat.width,
                newState.cat.position.x + dx * speed
              )
            ),
            y: Math.max(
              0,
              Math.min(
                CANVAS_HEIGHT - newState.cat.height,
                newState.cat.position.y + dy * speed
              )
            ),
          },
        };

        newState = { ...newState, cat: newCat };

        // Handle spacebar interaction
        if (keys.has(' ')) {
          newState = handleInteraction(newState);
        }

        // Check detection
        const detected = checkDetection(newState);
        if (detected && isSprinting) {
          newState.caught = true;
          newState.gameOver = true;
        }

        // Update human look direction (simple patrol)
        newState.humans = newState.humans.map((human) => ({
          ...human,
          lookDirection: (human.lookDirection + 0.01) % (2 * Math.PI),
        }));

        return newState;
      });

      animationFrameRef.current = requestAnimationFrame(gameLoop);
    };

    animationFrameRef.current = requestAnimationFrame(gameLoop);

    return () => {
      if (animationFrameRef.current) {
        cancelAnimationFrame(animationFrameRef.current);
      }
    };
  }, [keys, gameState.gameOver, handleInteraction, checkDetection]);

  // Render game
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    renderGame(ctx, gameState);
  }, [gameState]);

  const handleRestart = () => {
    setGameState(createInitialState());
    setKeys(new Set());
  };

  return (
    <div className="game-container">
      <div className="game-header">
        <h1>CATastrophe</h1>
        <div className="game-stats">
          <div className="stat">Score: {gameState.score}</div>
          <div className="stat">
            Status: {gameState.gameOver ? (gameState.caught ? 'CAUGHT!' : 'Game Over') : 'Playing'}
          </div>
        </div>
      </div>
      <canvas
        ref={canvasRef}
        width={CANVAS_WIDTH}
        height={CANVAS_HEIGHT}
        className="game-canvas"
      />
      <div className="game-controls">
        <div className="controls-section">
          <h3>Controls</h3>
          <p>⬆️⬇️⬅️➡️ or WASD - Move</p>
          <p>SPACEBAR - Interact/Destroy</p>
          <p>SHIFT - Sprint (risky!)</p>
        </div>
        <div className="controls-section">
          <h3>Tips</h3>
          <p>🎯 Get close to objects and press SPACE</p>
          <p>👀 Avoid human sight lines</p>
          <p>⚡ Sprinting increases detection risk</p>
        </div>
      </div>
      {gameState.gameOver && (
        <div className="game-over-overlay">
          <div className="game-over-modal">
            <h2>{gameState.caught ? '😿 Caught!' : 'Game Over'}</h2>
            <p>Final Score: {gameState.score}</p>
            <button onClick={handleRestart} className="restart-button">
              Play Again
            </button>
          </div>
        </div>
      )}
    </div>
  );
};

export default Game;
