/*
    Guess The Number (1 - 30) - Presentation Edition
    Features:
    - Menu + rules screen
    - Input validation (no crashing on letters)
    - Attempts, hints (higher/lower), warm/cold feedback
    - Scoreboard across rounds
    - Engaging messages + "$100 winner" screen
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MIN_NUM 1
#define MAX_NUM 30

// ---------- Utility ----------
static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

static void wait_for_enter(void) {
    printf("\nPress ENTER to continue...");
    clear_stdin();
    getchar();
}

static void print_line(char ch, int n) {
    for (int i = 0; i < n; i++) putchar(ch);
    putchar('\n');
}

static int read_int_in_range(const char *prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);

        if (scanf("%d", &value) != 1) {
            printf("  ⚠️  That wasn't a number. Try again.\n");
            clear_stdin();
            continue;
        }

        if (value < min || value > max) {
            printf("  ⚠️  Please enter a number between %d and %d.\n", min, max);
            clear_stdin();
            continue;
        }

        clear_stdin();
        return value;
    }
}

static int generate_secret(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// ---------- UI Screens ----------
static void show_title(void) {
    print_line('=', 58);
    printf("        🎯  GUESS THE NUMBER:  🎯\n");
    print_line('=', 58);
    printf("Rules:\n");
    printf("  - The secret number is between %d and %d.\n", MIN_NUM, MAX_NUM);
    printf("  - You keep guessing until you get it.\n");
    printf("  - Get it right and you win: 💵 $100 💵\n");
    print_line('-', 58);
}

static void show_menu(void) {
    printf("\nMENU\n");
    print_line('-', 20);
    printf("1) Start Game\n");
    printf("2) How to Play\n");
    printf("3) Exit\n");
}

static void how_to_play(void) {
    print_line('-', 58);
    printf("HOW TO PLAY\n");
    print_line('-', 58);
    printf("1) Choose 'Start Game'\n");
    printf("2) Enter a guess between %d and %d\n", MIN_NUM, MAX_NUM);
    printf("3) The game will tell you:\n");
    printf("   - Higher / Lower hints\n");
    printf("   - Warm/Cold feedback based on closeness\n");
    printf("4) Keep going until you win $100!\n");
    print_line('-', 58);
    wait_for_enter();
}

// Warm/Cold feedback based on distance
static void feedback(int guess, int secret) {
    int diff = guess - secret;
    if (diff < 0) diff = -diff;

    if (diff == 0) return;

    if (diff <= 2) {
        printf("🔥 SUPER HOT! You're extremely close.\n");
    } else if (diff <= 5) {
        printf("🌡️  Warm... you're getting closer.\n");
    } else if (diff <= 10) {
        printf("❄️  Cold... not too close yet.\n");
    } else {
        printf("🧊 ICE COLD... you're far off.\n");
    }
}

static void winner_screen(int attempts, int best_attempts) {
    print_line('=', 58);
    printf("🏆 CONGRATULATIONS! YOU GOT IT RIGHT!\n");
    printf("💵 You just won $100! 💵\n");
    print_line('-', 58);
    printf("Attempts this round: %d\n", attempts);
    if (best_attempts > 0) {
        printf("Best attempt so far: %d\n", best_attempts);
    }
    print_line('=', 58);
}

// ---------- Game Loop ----------
static void play_game(int *rounds_played, int *best_attempts) {
    int secret = generate_secret(MIN_NUM, MAX_NUM);
    int attempts = 0;

    print_line('-', 58);
    printf("🎲 A secret number has been chosen (%d to %d).\n", MIN_NUM, MAX_NUM);
    printf("Type your guess below!\n");
    print_line('-', 58);

    while (1) {
        int guess = read_int_in_range("👉 Take a guess: ", MIN_NUM, MAX_NUM);
        attempts++;

        if (guess == secret) {
            (*rounds_played)++;
            if (*best_attempts == 0 || attempts < *best_attempts) {
                *best_attempts = attempts;
            }
            winner_screen(attempts, *best_attempts);
            break;
        }

        if (guess < secret) {
            printf("📈 Too low! Try a higher number.\n");
        } else {
            printf("📉 Too high! Try a lower number.\n");
        }

        feedback(guess, secret);
        printf("Attempts so far: %d\n\n", attempts);
        printf("❗ Not correct yet — try again!\n\n");
    }

    // Replay option
    while (1) {
        char choice;
        printf("\nPlay again? (y/n): ");
        if (scanf(" %c", &choice) != 1) {
            clear_stdin();
            continue;
        }
        clear_stdin();

        choice = (char)tolower((unsigned char)choice);
        if (choice == 'y') {
            play_game(rounds_played, best_attempts);
            return;
        } else if (choice == 'n') {
            printf("\nReturning to menu...\n");
            return;
        } else {
            printf("Please type 'y' or 'n'.\n");
        }
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    int rounds_played = 0;
    int best_attempts = 0;

    while (1) {
        show_title();
        printf("Scoreboard: Rounds won = %d | Best attempts = %s\n",
               rounds_played,
               (best_attempts == 0 ? "N/A" : "set"));

        if (best_attempts != 0) {
            printf("            Best attempts value = %d\n", best_attempts);
        }

        show_menu();

        int option = read_int_in_range("\nChoose an option (1-3): ", 1, 3);

        if (option == 1) {
            play_game(&rounds_played, &best_attempts);
            wait_for_enter();
        } else if (option == 2) {
            how_to_play();
        } else {
            print_line('=', 58);
            printf("Thanks for playing! 🎭 Come back for your $100 win!\n");
            print_line('=', 58);
            break;
        }
    }

    return 0;
}