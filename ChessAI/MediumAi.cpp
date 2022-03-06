//
// Created by rasyt on 12/21/2021.
//

#include "MediumAi.h"



void Chess::MediumAi::setPawnevalsquares() {
    pawnevalsquares = {{0,  0,  0,  0,  0,  0,  0,  0},
                       {50, 50, 50, 50, 50, 50, 50, 50},
                       {10, 10, 20, 30, 30, 20, 10, 10},
                       {5,  5, 10, 25, 25, 10,  5,  5},
                       {0,  0,  0, 20, 20,  0,  0,  0},
                       {5, -5,-10,  0,  0,-10, -5,  5},
                       {5, 10, 10,-20,-20, 10, 10,  5},
                       {0,  0,  0,  0,  0,  0,  0,  0}};
};



void Chess::MediumAi::setKnightevalsquares() {
    knightevalsquares = {{-50,-40,-30,-30,-30,-30,-40,-50},
                         {-40,-20,  0,  0,  0,  0,-20,-40},
                         {-30,  0, 10, 15, 15, 10,  0,-30},
                         {-30,  5, 15, 20, 20, 15,  5,-30},
                         {-30,  0, 15, 20, 20, 15,  0,-30},
                         {-30,  5, 10, 15, 15, 10,  5,-30},
                         {-40,-20,  0,  5,  5,  0,-20,-40},
                         {-50,-40,-30,-30,-30,-30,-40,-50}};
}


void Chess::MediumAi::setBishopevalsquares() {
    bishopevalsquares = {{-20,-10,-10,-10,-10,-10,-10,-20},
                         {-10,  0,  0,  0,  0,  0,  0,-10},
                         {-10,  0,  5, 10, 10,  5,  0,-10},
                         {-10,  5,  5, 10, 10,  5,  5,-10},
                         {-10,  0, 10, 10, 10, 10,  0,-10},
                         {-10, 10, 10, 10, 10, 10, 10,-10},
                         {-10,  5,  0,  0,  0,  0,  5,-10},
                         {-20,-10,-10,-10,-10,-10,-10,-20}};
}



void Chess::MediumAi::setRookevalsquares() {
    rookevalsquares = {{0,  0,  0,  0,  0,  0,  0,  0},
                       {5, 10, 10, 10, 10, 10, 10,  5},
                       {-5,  0,  0,  0,  0,  0,  0, -5},
                       {-5,  0,  0,  0,  0,  0,  0, -5},
                       {-5,  0,  0,  0,  0,  0,  0, -5},
                       {-5,  0,  0,  0,  0,  0,  0, -5},
                       {-5,  0,  0,  0,  0,  0,  0, -5},
                       {0,  0,  0,  5,  5,  0,  0,  0}};
}


void Chess::MediumAi::setQueenevalsquares() {
    queenevalsquares = {{-20,-10,-10, -5, -5,-10,-10,-20},
            {-10,  0,  0,  0,  0,  0,  0,-10},
            {-10,  0,  5,  5,  5,  5,  0,-10},
            {-5,  0,  5,  5,  5,  5,  0, -5},
            {0,  0,  5,  5,  5,  5,  0, -5},
            {-10,  5,  5,  5,  5,  5,  0,-10},
            {-10,  0,  5,  0,  0,  0,  0,-10},
            {-20,-10,-10, -5, -5,-10,-10,-20}};
}


void Chess::MediumAi::setKingevalsquares() {
    kingevalsquares = {{-50,-40,-30,-20,-20,-30,-40,-50},
                       {-30,-20,-10,  0,  0,-10,-20,-30},
                       {-30,-10, 20, 30, 30, 20,-10,-30},
                       {-30,-10, 30, 40, 40, 30,-10,-30},
                       {-30,-10, 30, 40, 40, 30,-10,-30},
                       {-30,-10, 20, 30, 30, 20,-10,-30},
                       {-30,-30,  0,  0,  0,  0,-30,-30},
                       {-50,-30,-30,-30,-30,-30,-30,-50}};
}


int Chess::MediumAi::minimaxalphabeta(std::vector<std::vector<char>> underboard, std::vector<std::vector<Pieces>> thepieces, int depth, int alpha, int beta, std::string maximizingPlayer, Game& thegame){
    //bool Chess::Game::checkmate(std::string colorturn, int endposy, int endposx) {
    if (depth == 0 or thegame.checkmate(maximizingPlayer, -9000, 9000))
    {
        return staticeval(underboard, thepieces);
    }
    if (maximizingPlayer == "white")
    {
        int maxEvaluation = -INT_MAX;
        //check if im currently able to castle on both ways and add that move in
        std::vector<std::pair<char, std::vector<int>>> possiblemoves = getallpossiblemoves(maximizingPlayer, underboard, thepieces);
        //getallpossiblemoves will handle move types like enpassant,
        Player matter;
        for (std::pair<char, std::vector<int>>& move : possiblemoves)
        {
            std::vector<std::vector<char>> prevboard = underboard;
            std::vector<std::vector<Pieces>> prevpieces = thepieces;
            thegame.move(underboard, thepieces, maximizingPlayer, move.second[0], move.second[1], move.second[3], move.second[4], matter);
            int eval = minimaxalphabeta(underboard, thepieces, depth - 1, alpha, beta, maximizingPlayer, thegame);
            underboard = prevboard;
            thepieces = prevpieces;
            maxEvaluation = std::max(maxEvaluation, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
            {
                break;
            }
        }
        return maxEvaluation;

    } else
    {
        int minEvaluation = INT_MAX;
        std::vector<std::pair<char, std::vector<int>>> possiblemoves = getallpossiblemoves(maximizingPlayer, underboard, thepieces);
        Player matter;
        for (std::pair<char, std::vector<int>>& move : possiblemoves)
        {
            std::vector<std::vector<char>> prevboard = underboard;
            std::vector<std::vector<Pieces>> prevpieces = thepieces;
            thegame.move(underboard, thepieces, maximizingPlayer, move.second[0], move.second[1], move.second[3], move.second[4], matter);
            int eval = minimaxalphabeta(underboard, thepieces, depth - 1, alpha, beta, maximizingPlayer, thegame);
            underboard = prevboard;
            thepieces = prevpieces;
            minEvaluation = std::min(minEvaluation, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) 
            {
                break;
            }
        }
        return minEvaluation;
    }
}




std::vector<std::pair<char, std::vector<int>>> Chess::MediumAi::getallpossiblemoves(std::string color, std::vector<std::vector<char>> underboard, std::vector<std::vector<Pieces>> thepieces) {
    std::vector<std::pair<char, std::vector<int>>> allpossiblemoves;
    Pawn pawnobj(0, 0, -9000, -9000);
    Rook rookobj(0, 0, -9000, -9000);
    Knight knightobj(0, 0, -9000, -9000);
    King kingobj(0, 0, -9000, -9000);
    Queen queenobj(0, 0, -9000, -9000);
    Bishop bishopobj(0, 0, -9000, -9000);
    //special moves I need to add:
    //castling, en passant, promotion
        for (int i = 0; i < underboard.size(); i++)
        {
            for (int j = 0; j < underboard[0].size(); i++)
            {
                if (thepieces[i][j].getcolor() == color and underboard[i][j] != ' ')
                {
                    switch (underboard[i][j])
                    {
                        case 'P':
                            pawnobj.setstartpos(i, j);
                            pawnobj.getpossiblemoves();
                            for (auto item : pawnobj.getpossiblemoves())
                            {
                                std::vector<int> moves;
                                moves.push_back(i);
                                moves.push_back(j);
                                moves.push_back(item.first);
                                moves.push_back(item.second);
                                std::pair<char, std::vector<int>> movecomb = std::make_pair(underboard[i][j], moves);
                                allpossiblemoves.push_back(movecomb);
                            }
                            break;
                        case 'R':
                            rookobj.setstartpos(i, j);
                            rookobj.getpossiblemoves();
                            for (auto item : rookobj.getpossiblemoves())
                            {
                                std::vector<int> moves;
                                moves.push_back(i);
                                moves.push_back(j);
                                moves.push_back(item.first);
                                moves.push_back(item.second);
                                std::pair<char, std::vector<int>> movecomb = std::make_pair(underboard[i][j], moves);
                                allpossiblemoves.push_back(movecomb);
                            }

                            break;
                        case 'K':
                            knightobj.setstartpos(i, j);
                            knightobj.getpossiblemoves();
                            for (auto item : knightobj.getpossiblemoves())
                            {
                                std::vector<int> moves;
                                moves.push_back(i);
                                moves.push_back(j);
                                moves.push_back(item.first);
                                moves.push_back(item.second);
                                std::pair<char, std::vector<int>> movecomb = std::make_pair(underboard[i][j], moves);
                                allpossiblemoves.push_back(movecomb);
                            }
                            break;
                        case 'B':
                            bishopobj.setstartpos(i, j);
                            bishopobj.getpossiblemoves();
                            for (auto item : bishopobj.getpossiblemoves())
                            {
                                std::vector<int> moves;
                                moves.push_back(i);
                                moves.push_back(j);
                                moves.push_back(item.first);
                                moves.push_back(item.second);
                                std::pair<char, std::vector<int>> movecomb = std::make_pair(underboard[i][j], moves);
                                allpossiblemoves.push_back(movecomb);
                            }
                            break;
                        case 'Q':
                            queenobj.setstartpos(i, j);
                            queenobj.getpossiblemoves();
                            for (auto item : queenobj.getpossiblemoves())
                            {
                                std::vector<int> moves;
                                moves.push_back(i);
                                moves.push_back(j);
                                moves.push_back(item.first);
                                moves.push_back(item.second);
                                std::pair<char, std::vector<int>> movecomb = std::make_pair(underboard[i][j], moves);
                                allpossiblemoves.push_back(movecomb);
                            }

                            break;
                        case 'A':
                            kingobj.setstartpos(i, j);
                            kingobj.getpossiblemoves();
                            for (auto item : kingobj.getpossiblemoves())
                            {
                                std::vector<int> moves;
                                moves.push_back(i);
                                moves.push_back(j);
                                moves.push_back(item.first);
                                moves.push_back(item.second);
                                std::pair<char, std::vector<int>> movecomb = std::make_pair(underboard[i][j], moves);
                                allpossiblemoves.push_back(movecomb);
                            }
                            break;
                    }
                }
            }
        }



}





