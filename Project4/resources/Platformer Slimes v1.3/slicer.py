from PIL import Image

def slice_sprite_sheet(input_path, output_folder, sprite_width, sprite_height, start_x=0, start_y=0, color=""):
    # Open the sprite sheet
    sprite_sheet = Image.open(input_path)

    # Get the dimensions of the sprite sheet
    sheet_width, sheet_height = sprite_sheet.size

    # Define the number of rows and columns
    rows = sheet_height // sprite_height
    cols = sheet_width // sprite_width

    # Loop through each row and column to slice the sprite sheet
    for row in range(rows):
        if row in [1,2,4]:
            for col in range(cols):
                if col < 3:
                    # Calculate the coordinates of the current sprite
                    left = col * sprite_width + start_x if row != 4 else (col+3) * sprite_width + 16 + start_x if col != 1 else (col+3) * sprite_width + 16 + start_x - 16 
                    upper = row * sprite_height + start_y
                    
                    right = left + sprite_width if row != 4 else left + sprite_width + 16
                    lower = upper + sprite_height if row != 4 else upper + sprite_height + 16
                    enemyStateSet = {1:"WL","WL":"WR",2:"DL","DL":"DR",4:"FL","FL":"FR"}
                    # Crop the sprite from the sprite sheet
                    
                    sprite = sprite_sheet.crop((left, upper, right, lower))
                    spriteReversed = sprite.transpose(Image.FLIP_LEFT_RIGHT)
                    # Save the sliced sprite to the output folder
                    sprite.save(f"{output_folder}/{color}_Slime_{enemyStateSet[row]}_{col}.png")
                    spriteReversed.save(f"{output_folder}/{color}_Slime_{enemyStateSet[enemyStateSet[row]]}_{col}.png")
    

# Example usage with starting x and y points
color = "Red"
input_sprite_sheet = f"{color} Slime Spritesheet.png"
output_folder = color
sprite_width = 16
sprite_height = 16
start_x = 48  # Adjust as needed
start_y = 0   # Adjust as needed

slice_sprite_sheet(input_sprite_sheet, output_folder, sprite_width, sprite_height, start_x, start_y, color)
