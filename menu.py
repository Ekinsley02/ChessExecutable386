import json

import os
import pygame
import sys  # Added to use sys.exit()


import constants
#############################

pygame.init()
pygame.font.init()

import os
import sys

def resource_path(relative_path):
    """ Get the absolute path to a resource, works for both dev and PyInstaller .exe """
    if getattr(sys, 'frozen', False):  # Check if running in PyInstaller bundle
        base_path = sys._MEIPASS
    else:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)


# Initialize dictionary to track hover states of buttons
last_hover_state = {}

# Set up display
WIN = pygame.display.set_mode((constants.WIDTH, constants.HEIGHT))
pygame.display.set_caption("Your Game Title")

menu_font_path = resource_path("images/menu/menu_font.ttf")
background_image_path = resource_path("images/menu/menu_background.png")
logo_image_path = resource_path("images/menu/menu_logo.png")
button_image_path = resource_path("images/menu/menu_button.png")
button_hover_path = resource_path("images/menu/menu_button_hover.png")
custom_font_path = resource_path("images/menu/menu_font.ttf")
title_font_path = resource_path("images/menu/menu_title_font1.ttf")
button_sound_path = resource_path("images/menu/button_click.wav")
hover_sound_path = resource_path("images/menu/button_hover.wav")
typing_sound_path = resource_path("images/menu/typing_sound.wav")
menu_music_path = resource_path("images/menu/menu_sound.wav")

# Load assets
background_image = pygame.image.load(background_image_path)
logo_image = pygame.image.load(logo_image_path)
button_image = pygame.image.load(button_image_path)
button_hover_image = pygame.image.load(button_hover_path)
custom_font = pygame.font.Font(custom_font_path, 36)
title_font = pygame.font.Font(title_font_path, 72)
button_sound = pygame.mixer.Sound(button_sound_path)
hover_sound = pygame.mixer.Sound(hover_sound_path)
typing_sound = pygame.mixer.Sound(typing_sound_path)
menu_music = menu_music_path


typing_sound.set_volume(0.1)  # Set the typing sound to 30% volume
hover_sound.set_volume(0.3)  # Set the typing sound to 30% volume
button_sound.set_volume(0.3)  # Set the typing sound to 30% volume

pygame.mixer.music.load(menu_music)
pygame.mixer.music.set_volume(0.3)  # Set volume (0.0 to 1.0)
pygame.mixer.music.play(-1)  # Play in a loop indefinitely

# JSON file to store user credentials
CREDENTIALS_FILE = "user_data.json"

# Initialize the credentials file if it doesn't exist
if not os.path.exists(CREDENTIALS_FILE):
    with open(CREDENTIALS_FILE, "w") as file:
        json.dump({}, file)

def load_credentials():
    """Load user credentials from the JSON file."""
    with open(CREDENTIALS_FILE, "r") as file:
        return json.load(file)

def save_credentials(credentials):
    """Save user credentials to the JSON file."""
    with open(CREDENTIALS_FILE, "w") as file:
        json.dump(credentials, file)



###################   END of User SQL Implementation    ###################

def fade_in(win, width, height):
    fade_surface = pygame.Surface((width, height))
    fade_surface.fill((0, 0, 0))
    for alpha in range(255, -1, -5):
        fade_surface.set_alpha(alpha)
        win.blit(fade_surface, (0, 0))
        pygame.display.update()
        pygame.time.delay(5)

def fade_out(win, width, height):
    fade_surface = pygame.Surface((width, height))
    fade_surface.fill((0, 0, 0))
    for alpha in range(0, 256, 5):
        fade_surface.set_alpha(alpha)
        win.blit(fade_surface, (0, 0))
        pygame.display.update()
        pygame.time.delay(5)

def get_text_input(WIN, WIDTH, HEIGHT, prompt, y_offset):
    """Get text input from the user in the Pygame window using enhanced visuals."""
    font = pygame.font.Font(menu_font_path, 42)  # Use a larger font for better visibility
    input_rect = pygame.Rect(WIDTH // 2 - 150, HEIGHT // 2 + y_offset, 300, 50)
    color_active_border = constants.COLORS["BLACK"]  # Cream color for the border when active
    color_inactive_border = constants.COLORS["WHITE"]  # Black border when not active
    color = color_inactive_border
    active = False
    user_text = ""

    back_button_rect = pygame.Rect(20, 20, 100, 40)  # Position and size for the "Back" button

    run = True
    while run:
        # Draw background to keep consistency
        WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

        # Display the prompt with drop shadow
        shadow_offset = 2
        shadow_color = (0, 0, 0)  # Black for shadow
        prompt_surface_shadow = font.render(prompt, True, shadow_color)
        prompt_surface = font.render(prompt, True, constants.COLORS["WHITE"])

        # Draw shadow
        WIN.blit(prompt_surface_shadow, (WIDTH // 2 - prompt_surface.get_width() // 2 + shadow_offset, HEIGHT // 2 - 100 + shadow_offset))

        # Draw the main text on top of shadow
        WIN.blit(prompt_surface, (WIDTH // 2 - prompt_surface.get_width() // 2, HEIGHT // 2 - 100))

        # Draw input box with white background
        pygame.draw.rect(WIN, constants.COLORS["WHITE"], input_rect)  # Draw the white background for the input box
        pygame.draw.rect(WIN, color, input_rect, 2)  # Draw the border around the input box

        # Render the text inside the input box, vertically centered
        text_surface = font.render(user_text, True, constants.COLORS["BLACK"])
        text_rect = text_surface.get_rect(center=(input_rect.x + input_rect.width // 2, input_rect.y + input_rect.height // 2))
        WIN.blit(text_surface, text_rect)

        # Draw the "Back" button with consistent style
        back_hover = draw_button(WIN, "Back", back_button_rect.x, back_button_rect.y, 100, 40, button_image, button_hover_image, constants.COLORS["WHITE"])

        pygame.display.flip()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if back_hover:
                    button_sound.play()
                    return None  # Return None to indicate "Back" was clicked
                # Toggle active state if clicked inside the input box
                if input_rect.collidepoint(event.pos):
                    active = not active
                else:
                    active = False
                color = color_active_border if active else color_inactive_border
            if event.type == pygame.KEYDOWN:
                if active:
                    if event.key == pygame.K_RETURN:
                        run = False  # End input on Enter
                    elif event.key == pygame.K_BACKSPACE:
                        user_text = user_text[:-1]
                    else:
                        typing_sound.play()  # Play the typing sound
                        user_text += event.unicode

    return user_text

def signup(WIN, WIDTH, HEIGHT):
    """Allow the user to create a new account with consistent styling."""
    run = True

    # Define button dimensions for the Back button
    back_button_width = 150
    back_button_height = 50
    back_button_rect = pygame.Rect(20, 20, back_button_width, back_button_height)  # Back button dimensions

    while run:
        # Collect Username
        username = get_text_input(WIN, WIDTH, HEIGHT, "Enter Username:", -50)
        if username is None:
            # User clicked "Back"
            return False  # Return False to indicate to go back

        # Collect Password
        password = get_text_input(WIN, WIDTH, HEIGHT, "Enter Password:", -50)
        if password is None:
            # User clicked "Back"
            return False  # Return False to indicate to go back

        # Handle Results
        credentials = load_credentials()

        # Username already exists case
        if username in credentials:
            print("Username already exists. Please try again.")
            error_text_line1 = "Username already exists!"
            error_text_line2 = "Please wait and then try again."

            WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

            # Draw error message background box
            text_box_width = 500
            text_box_height = 120
            box_x = (WIDTH - text_box_width) // 2
            box_y = HEIGHT // 2 - 40

            text_box_surface = pygame.Surface((text_box_width, text_box_height))
            text_box_surface.set_alpha(180)
            text_box_surface.fill((0, 0, 0))
            WIN.blit(text_box_surface, (box_x, box_y))

            # Render the error text on two lines
            error_font = pygame.font.Font(menu_font_path, 36)

            error_surface_line1 = error_font.render(error_text_line1, True, constants.COLORS["WHITE"])
            WIN.blit(
                error_surface_line1,
                (WIDTH // 2 - error_surface_line1.get_width() // 2, box_y + 10)
            )

            error_surface_line2 = error_font.render(error_text_line2, True, constants.COLORS["WHITE"])
            WIN.blit(
                error_surface_line2,
                (WIDTH // 2 - error_surface_line2.get_width() // 2, box_y + 60)
            )

            pygame.display.update()
            pygame.time.wait(2000)
        else:
            # Save credentials
            credentials[username] = password
            save_credentials(credentials)
            print("Account created successfully!")
            success_text_line1 = "Account created successfully!"
            success_text_line2 = "One moment please."

            WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

            # Draw success message background box
            text_box_width = 500
            text_box_height = 120
            box_x = (WIDTH - text_box_width) // 2
            box_y = HEIGHT // 2 - 40

            text_box_surface = pygame.Surface((text_box_width, text_box_height))
            text_box_surface.set_alpha(180)
            text_box_surface.fill((0, 0, 0))
            WIN.blit(text_box_surface, (box_x, box_y))

            success_surface_line1 = custom_font.render(success_text_line1, True, constants.COLORS["WHITE"])
            WIN.blit(
                success_surface_line1,
                (WIDTH // 2 - success_surface_line1.get_width() // 2, box_y + 10)
            )

            success_surface_line2 = custom_font.render(success_text_line2, True, constants.COLORS["WHITE"])
            WIN.blit(
                success_surface_line2,
                (WIDTH // 2 - success_surface_line2.get_width() // 2, box_y + 60)
            )

            pygame.display.update()
            pygame.time.wait(2000)
            run = False

    return True

def color_customization_menu(WIN, WIDTH, HEIGHT):
    running = True

    # Pre-defined color themes
    color_themes = {
        "classic": {
            "CREAM": [238, 238, 210],
            "BROWN": [118, 150, 86],
            "HIGHLIGHT": [255, 255, 0, 100],
            "SELECTED": [255, 0, 0],
        },
        "ocean": {
            "CREAM": [173, 216, 230],
            "BROWN": [0, 105, 148],
            "HIGHLIGHT": [0, 255, 255, 100],
            "SELECTED": [255, 20, 147],
        },
        "sunset": {
            "CREAM": [255, 228, 196],
            "BROWN": [255, 99, 71],
            "HIGHLIGHT": [255, 140, 0, 100],
            "SELECTED": [255, 69, 0],
        },
        "forest": {
            "CREAM": [186, 235, 174],
            "BROWN": [34, 139, 34],
            "HIGHLIGHT": [154, 205, 50, 100],
            "SELECTED": [85, 107, 47],
        },
        "midnight": {
            "CREAM": [25, 25, 112],
            "BROWN": [75, 0, 130],
            "HIGHLIGHT": [123, 104, 238, 100],
            "SELECTED": [72, 61, 139],
        },
        "galaxy": {
            "CREAM": [138, 43, 226],
            "BROWN": [0, 0, 128],
            "HIGHLIGHT": [75, 0, 130, 100],
            "SELECTED": [255, 20, 147],
        }
    }

    theme_names = list(color_themes.keys())
    selected_theme_index = 0  # Start with the first theme as default
    current_theme = color_themes[theme_names[selected_theme_index]]

    # Use a more consistent font size and style
    font = pygame.font.Font(menu_font_path, 38)
    label_font = pygame.font.Font(menu_font_path, 32)  # Smaller font for labels to maintain distinction

    while running:
        # Draw Background
        WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

        # Add semi-transparent background box for text to improve readability
        text_box_surface = pygame.Surface((WIDTH, 150))
        text_box_surface.set_alpha(150)  # Adjust transparency level
        text_box_surface.fill((0, 0, 0))  # Black color for the box
        WIN.blit(text_box_surface, (0, 0))  # Position it at the top to cover the labels

        # Render Instructions at the Top
        instruction_surface = font.render('Choose a Color Theme', True, constants.COLORS["WHITE"])
        WIN.blit(instruction_surface, (WIDTH // 2 - instruction_surface.get_width() // 2, 10))

        # Render Current Color Theme
        theme_label = font.render(f'Current Theme: {theme_names[selected_theme_index].capitalize()}', True, constants.COLORS["WHITE"])
        WIN.blit(theme_label, (WIDTH // 2 - theme_label.get_width() // 2, 70))

        # Render Theme Change Button
        theme_button_width = 250
        theme_button_height = 50
        theme_button_x = WIDTH // 2 - theme_button_width // 2
        theme_button_y = 140

        theme_hover = draw_button(
            WIN, "Change Theme", theme_button_x, theme_button_y, theme_button_width, theme_button_height,
            button_image, button_hover_image, constants.COLORS["WHITE"]
        )

        # Display the colors from the current theme
        box_y_position = 220
        box_spacing = 150

        # Draw Light Color Box with Shadow
        light_color_box = pygame.Rect(50, box_y_position, 100, 100)
        shadow_rect = light_color_box.move(5, 5)
        pygame.draw.rect(WIN, (50, 50, 50), shadow_rect, border_radius=10)
        pygame.draw.rect(WIN, current_theme["CREAM"], light_color_box, border_radius=10)

        # Draw Dark Color Box with Shadow
        dark_color_box = pygame.Rect(50 + box_spacing + 100, box_y_position, 100, 100)
        shadow_rect = dark_color_box.move(5, 5)
        pygame.draw.rect(WIN, (50, 50, 50), shadow_rect, border_radius=10)
        pygame.draw.rect(WIN, current_theme["BROWN"], dark_color_box, border_radius=10)

        # Draw Highlight Color Box with Shadow
        highlight_color_box = pygame.Rect(50 + 2 * (box_spacing + 100), box_y_position, 100, 100)
        shadow_rect = highlight_color_box.move(5, 5)
        pygame.draw.rect(WIN, (50, 50, 50), shadow_rect, border_radius=10)
        pygame.draw.rect(WIN, current_theme["HIGHLIGHT"], highlight_color_box, border_radius=10)

        # Add Labels Under Each Box with Spacing
        light_label = label_font.render("Light Color", True, constants.COLORS["WHITE"])
        dark_label = label_font.render("Dark Color", True, constants.COLORS["WHITE"])
        highlight_label = label_font.render("Highlight Color", True, constants.COLORS["WHITE"])

        WIN.blit(light_label, (50 + (100 - light_label.get_width()) // 2, box_y_position + 110))
        WIN.blit(dark_label, (50 + box_spacing + 100 + (100 - dark_label.get_width()) // 2, box_y_position + 110))
        WIN.blit(highlight_label, (50 + 2 * (box_spacing + 100) + (100 - highlight_label.get_width()) // 2, box_y_position + 110))

        # Render Save Button with Shadow and hover effects
        save_button_width = 180  # Increased button width for better prominence
        save_button_height = 60  # Increased button height
        save_button_x = WIDTH // 2 - save_button_width // 2
        save_button_y = HEIGHT - 120  # Raised it up a bit for a better visual fit

        # Draw shadow for Save button
        save_button_rect = pygame.Rect(save_button_x, save_button_y, save_button_width, save_button_height)
        shadow_rect = save_button_rect.move(5, 5)
        pygame.draw.rect(WIN, (50, 50, 50), shadow_rect, border_radius=15)

        # Draw button with the images
        save_hover = draw_button(
            WIN, "Save", save_button_x, save_button_y, save_button_width, save_button_height,
            button_image, button_hover_image, constants.COLORS["WHITE"]
        )

        # Event Handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                mouse_pos = pygame.mouse.get_pos()

                # Click to change theme
                if theme_hover:
                    button_sound.play()
                    selected_theme_index = (selected_theme_index + 1) % len(theme_names)
                    # Update the current theme preview
                    current_theme = color_themes[theme_names[selected_theme_index]]

                # Click the Save Button
                elif save_hover:
                    button_sound.play()
                    # Update the COLORS dictionary with the selected theme
                    constants.COLORS.update(current_theme)
                    running = False

        pygame.display.flip()


def login(WIN, WIDTH, HEIGHT, game_state, logged_in):
    """Prompt the user to log in with consistent styling."""
    run = True
    while run:
        # Use consistent y-offsets for username and password inputs
        username = get_text_input(WIN, WIDTH, HEIGHT, "Enter Username:", -50)
        if username is None:
            # User clicked "Back"
            return logged_in, False  # Return logged_in and False to indicate to go back

        password = get_text_input(WIN, WIDTH, HEIGHT, "Enter Password:", -50)
        if password is None:
            # User clicked "Back"
            return logged_in, False  # Return logged_in and False to indicate to go back

        # Load credentials and validate login
        credentials = load_credentials()
        if username in credentials and credentials[username] == password:
            print("Login successful!")
            success_text_line1 = "Login successful!"
            success_text_line2 = "Starting game..."

            # Draw the background
            WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

            # Add a semi-transparent background box for readability
            text_box_width = 500
            text_box_height = 120
            box_x = (WIDTH - text_box_width) // 2
            box_y = HEIGHT // 2 - 40

            text_box_surface = pygame.Surface((text_box_width, text_box_height))
            text_box_surface.set_alpha(180)  # Adjust transparency
            text_box_surface.fill((0, 0, 0))  # Black color for the background

            WIN.blit(text_box_surface, (box_x, box_y))

            # Render the success text on two lines
            success_font = pygame.font.Font(menu_font_path, 36)

            # Line 1 - "Login successful!"
            success_surface_line1 = success_font.render(success_text_line1, True, constants.COLORS["WHITE"])
            WIN.blit(
                success_surface_line1,
                (WIDTH // 2 - success_surface_line1.get_width() // 2, box_y + 10)
            )

            # Line 2 - "Starting game..."
            success_surface_line2 = success_font.render(success_text_line2, True, constants.COLORS["WHITE"])
            WIN.blit(
                success_surface_line2,
                (WIDTH // 2 - success_surface_line2.get_width() // 2, box_y + 60)
            )

            pygame.display.update()
            pygame.time.wait(2000)
            logged_in = True  # Update the login status
            run = False
        else:
            print("Invalid username or password. Please try again.")
            error_text_line1 = "Invalid credentials!"
            error_text_line2 = "Please wait and try again."

            WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

            # Add a semi-transparent background box for readability
            text_box_width = 500
            text_box_height = 120
            box_x = (WIDTH - text_box_width) // 2
            box_y = HEIGHT // 2 - 40

            text_box_surface = pygame.Surface((text_box_width, text_box_height))
            text_box_surface.set_alpha(180)  # Adjust transparency
            text_box_surface.fill((0, 0, 0))  # Black color for the background

            WIN.blit(text_box_surface, (box_x, box_y))

            # Render the error text on two lines
            error_font = pygame.font.Font(menu_font_path, 36)

            # Line 1 - "Invalid credentials!"
            error_surface_line1 = error_font.render(error_text_line1, True, constants.COLORS["WHITE"])
            WIN.blit(
                error_surface_line1,
                (WIDTH // 2 - error_surface_line1.get_width() // 2, box_y + 10)
            )

            # Line 2 - "Please wait and try again."
            error_surface_line2 = error_font.render(error_text_line2, True, constants.COLORS["WHITE"])
            WIN.blit(
                error_surface_line2,
                (WIDTH // 2 - error_surface_line2.get_width() // 2, box_y + 60)
            )

            pygame.display.update()
            pygame.time.wait(2000)
    return logged_in, True  # Return logged_in and True to continue

def draw_button(win, text, x, y, width, height, default_image, hover_image, text_color):
    """Draw a button using different images for regular and hover states, with text centered inside."""

    # Track whether the hover sound should be played
    global last_hover_state

    # Get mouse position to determine hover
    mouse_pos = pygame.mouse.get_pos()
    button_rect = pygame.Rect(x, y, width, height)
    is_hovered = button_rect.collidepoint(mouse_pos)

    # Check if the hover state changed from non-hover to hover
    if is_hovered and not last_hover_state.get((x, y), False):
        hover_sound.play()  # Play sound when switching to hover state

    # Update the last hover state for the button
    last_hover_state[(x, y)] = is_hovered

    # Use hover image if hovered, otherwise use the default image
    button_surf = pygame.transform.smoothscale(
        hover_image if is_hovered else default_image,
        (width, height)
    )

    # Draw the button (image should cover the entire area without gaps)
    win.blit(button_surf, (x, y))

    # Render the text on top of the button
    text_surface = custom_font.render(text, True, text_color)
    text_rect = text_surface.get_rect(center=button_rect.center)
    win.blit(text_surface, text_rect)

    return is_hovered

### Function for Login Menu ###
def login_menu(WIN, WIDTH, HEIGHT, BROWN, CREAM, BLACK, WHITE, game_state, logged_in):
    """Display the login menu with enhanced visuals."""
    menu_run = True

    # Define new size for the logo
    logo_width = 114  # Adjusted to a reasonable size
    logo_height = 148  # Adjusted to keep the aspect ratio

    # Resize the logo to the desired dimensions
    resized_logo = pygame.transform.smoothscale(logo_image, (logo_width, logo_height))

    # Define button dimensions and positions
    button_width = 240  # Consistent width for all menus
    button_height = 75  # Consistent height for all menus
    vertical_gap = 35  # Space between buttons

    # Calculate initial vertical position for the buttons to be centered
    button_y_start = HEIGHT // 2 - (button_height * 1) - vertical_gap // 2
    button_x = (WIDTH - button_width) // 2

    while menu_run:
        # Draw background
        WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

        # Draw resized logo
        logo_x = 20
        logo_y = 20
        WIN.blit(resized_logo, (logo_x, logo_y))

        # Draw the title below the logo
        title_surface = title_font.render("chess2gether", True, WHITE)
        title_rect = title_surface.get_rect(center=(WIDTH // 2, logo_y + logo_height + 20))
        WIN.blit(title_surface, title_rect)

        # Draw Login button
        login_hover = draw_button(
            WIN, "Login", button_x, button_y_start, button_width, button_height,
            button_image, button_hover_image, WHITE
        )

        # Draw Sign Up button
        signup_hover = draw_button(
            WIN, "Sign Up", button_x, button_y_start + button_height + vertical_gap, button_width, button_height,
            button_image, button_hover_image, WHITE
        )

        pygame.display.update()

        # Event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if login_hover:
                    button_sound.play()
                    fade_out(WIN, WIDTH, HEIGHT)
                    logged_in, continue_to_menu = login(WIN, WIDTH, HEIGHT, game_state, logged_in)
                    fade_in(WIN, WIDTH, HEIGHT)
                    if logged_in and continue_to_menu:
                        game_state.state = 'MENU'
                        menu_run = False
                elif signup_hover:
                    button_sound.play()
                    fade_out(WIN, WIDTH, HEIGHT)
                    signup_completed = signup(WIN, WIDTH, HEIGHT)
                    fade_in(WIN, WIDTH, HEIGHT)

    return logged_in

def main_menu(WIN, WIDTH, HEIGHT, game_state, c_engine):
    """Display the main menu with balanced visuals."""
    print("Main Menu Active...")  # Debug
    menu_run = True

    # Define new size for the logo (if using)
    logo_width = 114  # Adjusted to a reasonable size
    logo_height = 148  # Adjusted to keep the aspect ratio

    # Resize the logo to the desired dimensions if needed
    resized_logo = pygame.transform.smoothscale(logo_image, (logo_width, logo_height))

    # Define button dimensions and positions
    button_width = 240  # General width for most buttons
    customize_button_width = 300  # Specific width for "Customize Colors" button
    button_height = 75  # Increased height to better fit text and feel balanced
    vertical_gap = 35  # Space between buttons to ensure enough spacing

    # Calculate initial vertical position for the buttons to be centered
    button_y_start = HEIGHT // 2 - (button_height * 2) - vertical_gap // 2

    # Horizontal centering of the buttons
    button_x = (WIDTH - button_width) // 2
    customize_button_x = (WIDTH - customize_button_width) // 2

    while menu_run:
        # Draw the consistent background
        WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

        # Optionally draw resized logo (or any other logo you want to use)
        logo_x = 20
        logo_y = 20
        WIN.blit(resized_logo, (logo_x, logo_y))

        # Draw the title below the logo
        title_surface = title_font.render("chess2gether", True, constants.COLORS["WHITE"])
        title_rect = title_surface.get_rect(center=(WIDTH // 2, logo_y + logo_height + 20))
        WIN.blit(title_surface, title_rect)

        # Draw Pass and Play button
        pass_play_hover = draw_button(
            WIN, "Pass and Play", button_x, button_y_start, button_width, button_height,
            button_image, button_hover_image, constants.COLORS["WHITE"]
        )

        # Draw TeacherAI button
        teacher_ai_hover = draw_button(
            WIN, "TeacherAI", button_x, button_y_start + button_height + vertical_gap, button_width, button_height,
            button_image, button_hover_image, constants.COLORS["WHITE"]
        )

        # Draw Online Play button
        online_play_hover = draw_button(
            WIN, "Online Play", button_x, button_y_start + 2 * (button_height + vertical_gap), button_width, button_height,
            button_image, button_hover_image, constants.COLORS["WHITE"]
        )

        # Draw Customize Colors button (new addition, with a wider width)
        customize_hover = draw_button(
            WIN, "Customize Colors", customize_button_x, button_y_start + 3 * (button_height + vertical_gap), customize_button_width, button_height,
            button_image, button_hover_image, constants.COLORS["WHITE"]
        )

        pygame.display.update()

        # Handle events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if pass_play_hover:
                    button_sound.play()
                    pygame.mixer.music.fadeout(2000)
                    print("Pass and Play Selected! Transitioning to GAME...")  # Debug
                    c_engine.process.stdin.write("0\n")
                    c_engine.process.stdin.flush()
                    game_state.state = 'GAME'  # Transition to GAME state
                    menu_run = False  # Exit the main menu loop
                    game_state.game_type = 0

                elif teacher_ai_hover:
                    button_sound.play()
                    pygame.mixer.music.fadeout(2000)
                    print("TeacherAI Selected! Transitioning to GAME...")  # Debug
                    c_engine.process.stdin.write("1\n")
                    c_engine.process.stdin.flush()
                    game_state.state = 'GAME'  # Transition to GAME state
                    menu_run = False  # Exit the main menu loop
                    game_state.game_type = 1

                elif online_play_hover:
                    # Display "Coming Soon" message directly here
                    button_sound.play()
                    
                    # Draw the background
                    WIN.blit(pygame.transform.scale(background_image, (WIDTH, HEIGHT)), (0, 0))

                    # Add a semi-transparent background box for readability
                    text_box_width = 500
                    text_box_height = 120
                    box_x = (WIDTH - text_box_width) // 2
                    box_y = HEIGHT // 2 - 40

                    text_box_surface = pygame.Surface((text_box_width, text_box_height))
                    text_box_surface.set_alpha(180)  # Adjust transparency
                    text_box_surface.fill((0, 0, 0))  # Black color for the background

                    WIN.blit(text_box_surface, (box_x, box_y))

                    # Render the "Coming Soon" message on two lines
                    coming_soon_font = pygame.font.Font(menu_font_path, 36)

                    # Line 1 - "Feature Coming Soon!"
                    coming_soon_surface_line1 = coming_soon_font.render("Feature Coming Soon!", True, constants.COLORS["WHITE"])
                    WIN.blit(
                        coming_soon_surface_line1,
                        (WIDTH // 2 - coming_soon_surface_line1.get_width() // 2, box_y + 10)
                    )

                    # Line 2 - "Please check back later."
                    coming_soon_surface_line2 = coming_soon_font.render("Please check back later.", True, constants.COLORS["WHITE"])
                    WIN.blit(
                        coming_soon_surface_line2,
                        (WIDTH // 2 - coming_soon_surface_line2.get_width() // 2, box_y + 60)
                    )

                    pygame.display.update()
                    pygame.time.wait(2000)
                elif customize_hover:
                    # Handle Customize Colors button
                    button_sound.play()
                    fade_out(WIN, WIDTH, HEIGHT)
                    color_customization_menu(WIN, WIDTH, HEIGHT)  # This will allow users to customize colors.
                    fade_in(WIN, WIDTH, HEIGHT)

    return game_state
