// lib/HumanPlayer.cpp

#include "lib/HumanPlayer.h"
#include "lib/KnowableState.h"

#include <ctype.h>

HumanPlayer::~HumanPlayer() {}
HumanPlayer::HumanPlayer(const AnnotatorPtr& annotator)
    : Strategy(annotator)
{}

Card getCardInput(const KnowableState& state)
{
    CardHand legal = state.LegalPlays();

    Card choice;
    printf("Choose a card:");
    while (true)
    {
        char* line = nullptr;
        size_t linecap = 0;
        ssize_t linelen = getline(&line, &linecap, stdin);
        assert(linelen >= 3); // 23456789TJQKA CDSH

        char rankChar = toupper(line[0]);
        char suitChar = toupper(line[1]);

        Rank rank;
        if (rankChar >= '2' && rankChar <= '9')
            rank = rankChar - '2';
        else if (rankChar == 'T')
            rank = kTen;
        else if (rankChar == 'J')
            rank = kJack;
        else if (rankChar == 'Q')
            rank = kQueen;
        else if (rankChar == 'K')
            rank = kKing;
        else if (rankChar == 'A')
            rank = kAce;
        else
        {
            printf("Not a valid rank char: %c \n", rankChar);
            continue;
        }

        Suit suit;
        if (suitChar == 'C')
            suit = kClubs;
        else if (suitChar == 'D')
            suit = kDiamonds;
        else if (suitChar == 'S')
            suit = kSpades;
        else if (suitChar == 'H')
            suit = kHearts;
        else
        {
            printf("Not a valid suit char: %c \n", suitChar);
            continue;
        }

        choice = CardFor(rank, suit);
        printf("You chose card: %s\n", NameOf(choice));

        if (legal.HasCard(choice))
            break;
        else
        {
            printf("But that is not a legal play!\n");
        }
    }

    return choice;
}

Card HumanPlayer::choosePlay(const KnowableState& state, const RandomGenerator& rng) const
{
    printf("Play %d\n", state.PlayNumber());

    int playInTrick = state.PlayInTrick();

    if (state.PointsPlayed() > 0)
    {
        if (state.PointsSplit())
        {
            printf("Points split\n");
        }
        else
        {
            for (int i = 0; i < 4; ++i)
            {
                int p = (state.PlayerLeadingTrick() + i) % 4;
                printf("%d ", state.GetScoreFor(p));
            }
            printf("\n");
        }
    }

    if (playInTrick == 0)
    {
        printf("You are leading the trick...\n");
    }
    else
    {
        for (int i = 0; i < playInTrick; ++i)
        {
            Card c = state.GetTrickPlay(i);
            printf(" %s ", NameOf(c));
        }
        printf("\n");
    }

    CardHand hand = state.CurrentPlayersHand();
    hand.Print();

    return getCardInput(state);
}
