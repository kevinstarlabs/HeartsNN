import re
import os

CARDS_IN_DECK = 52
NUM_SUITS = 4
NUM_RANKS = 13
NUM_PLAYERS = 4
PLAYS_PER_TRICK = 4

MOON_FLAGS_LEN = 4
POINTS_SO_FAR_LEN = PLAYS_PER_TRICK + 1 + MOON_FLAGS_LEN

# 7 features columns. The probabilities for each of the 4 players,
# plus 3 more: legal plays, high card in trick, and point value.
# Note that legal plays is a vector we must extract in the model.
# We have been placing it first, but we now make it the 5th column (column index 4),
# as that makes it easier to optionally set whether we include the two non-point suits (clubs & diamonds)
# redundantly when we combine the CNN output with the non-CNN features.
INPUT_FEATURES = NUM_PLAYERS + 3

EXTRA_FEATURES = 33

TOTAL_SCALAR_FEATURES = CARDS_IN_DECK*INPUT_FEATURES + POINTS_SO_FAR_LEN + EXTRA_FEATURES

DECK_SHAPE = (CARDS_IN_DECK,)

MAIN_INPUT_SHAPE = (TOTAL_SCALAR_FEATURES,)

SUITS_RANKS_SHAPE = (NUM_SUITS, NUM_RANKS)

POINTS_SO_FAR_SHAPE = (POINTS_SO_FAR_LEN,)

SCORES_SHAPE = DECK_SHAPE
WIN_TRICK_PROBS_SHAPE = DECK_SHAPE
MOON_CLASSES = 3
MOONPROBS_SHAPE = (MOON_CLASSES*CARDS_IN_DECK,)

number_re = re.compile(r'^(\d+)([KM]?)$')

default_vals = {
    'DECK_EPOCHS': 100,
    'DECK_BATCH': 64*1024,
    'DECK_SCORE': 1,
    'DECK_MOON': 1,
    'DECK_TRICK': 1,
}

def env_val(name) :
    if name in os.environ:
        s_val = os.environ[name]
        m = number_re.match(s_val)
        if not m:
            raise ValueError(f'Env var {name} must be integer with optional K or M suffix, given: {name}')
        val = int(m.group(1))
        if m.group(2) == 'K':
            val *= 1024
        elif m.group(2) == 'M':
            val *= 1024*1024
        print(f'Set {name} to {val}')
    else:
        val = default_vals[name]
    return val

BATCH = env_val('DECK_BATCH')
EPOCHS = env_val('DECK_EPOCHS')
SCORE = env_val('DECK_SCORE') == 1
MOON = env_val('DECK_MOON') == 1
TRICK = env_val('DECK_TRICK') == 1

MAIN_DATA = 'main_data'
EXPECTED_SCORE = 'expected_score'
WIN_TRICK_PROB = 'win_trick_prob'
MOON_PROB = 'moon_prob'
