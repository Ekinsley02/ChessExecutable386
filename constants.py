# constants.py

# Window size
WIDTH, HEIGHT = 800, 800

# RGB colors
COLORS = {
    "WHITE": [255, 255, 255],
    "BLACK": [0, 0, 0],
    "BROWN": [118, 150, 86],
    "CREAM": [238, 238, 210],
    "HIGHLIGHT": [0, 255, 0, 100],
    "SELECTED": [255, 0, 0]
}

# Highlight colors
HIGHLIGHT_COLOR = (0, 255, 0, 100)  # Highlight color for available moves
SELECTED_COLOR = (255, 0, 0)  # Red border for the selected piece

# Game states
LOGIN_MENU = 'LOGIN_MENU'
MENU = 'MENU'
GAME = 'GAME'
ENDED = 'ENDED'
