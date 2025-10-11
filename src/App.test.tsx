import React from 'react';
import { render, screen } from '@testing-library/react';
import App from './App';

test('renders CATastrophe game title', () => {
  render(<App />);
  const titleElement = screen.getByText(/catastrophe/i);
  expect(titleElement).toBeInTheDocument();
});

test('renders game controls', () => {
  render(<App />);
  const controlsElement = screen.getByText(/WASD - Move/i);
  expect(controlsElement).toBeInTheDocument();
});

test('renders game canvas', () => {
  const { container } = render(<App />);
  const canvas = container.querySelector('.game-canvas');
  expect(canvas).toBeInTheDocument();
});
