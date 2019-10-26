Simple Quiz game made with simple C, made for Linux

# Trivia Game - Project made by Adrian Brabete
Faculty of Automation and Computers 1st year
Making the application was, in my opinion, the most interesting task
I had to do it this semester. Learning the library together with
the development of the program proved to be much more interesting than I anticipated.
The working time was around 20 hours divided over several days.

Minimum requirements:
Although the game interface is scalable depending on the resolution of the window
terminal, there is also a minimum resolution for a correct display,
that being 80x32

Launching the game:
to launch the game use the command "./trivia Data / GameData.txt" or
"make run"

Menu navigation:
To navigate through the menu use the arrow keys and the selection is made
by pressing the "Enter" key. In the main menu there are the following buttons
"New Game", "Resume Game", which go from the menu to the difference game window
as "NewGame" resets the game window. The third button in the menu
"Quit" is quite explicit.

Game window:
After starting the game the user receives the question and the 4 variants of
answer. The selection of the variant is made in the menu by means of arrow keys and "Enter".
At the top is the score that increases directly in proportion to the number
of correct answers.
In order to help him, the player has 2 50/50 abilities and a skip that is
use by pressing the F key respectively S.

At the bottom is the window with icons for skills, date and time, and highscore.

I tried to make the game as visually pleasing as possible through effects and
different ASCII arts as icons. So at each run the game will display
one of the 4 logos that can be found in the Graphics file as a title.
