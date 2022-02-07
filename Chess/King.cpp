//
// Created by rasyt on 12/22/2021.
//

#include "King.h"


void Chess::King::CastleMovement(std::vector<std::vector<char>> &underboard,
                                 std::vector<std::vector<Pieces>> &thepieces) {
    if ((endposy == 7 or endposy == 0) and endposx == startposx - 2)
    {
        castleenact(underboard, thepieces, startposx - 4, endposx + 1);
    } else if ((endposy == 7 or endposy == 0) and endposx == startposx + 2)
    {
        castleenact(underboard, thepieces, startposx + 3, endposx - 1);
    }
}



void Chess::King::GenerateMoves(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    //top left, topright, uptop,
    //8 different directions.
    //for each of the direction coordinates, check if an opposing colored piece is watching the direction
    //if it isn't, we can push the move into the vector.
    //king cannot eat protected pieces;
    std::string kingcolor = color;
    std::vector<std::pair<int, int>> dirs = {{startposy - 1, startposx - 1}, {startposy - 1, startposx}, {startposy - 1, startposx + 1}, {startposy, startposx - 1}, {startposy, startposx + 1},
                                             {startposy + 1, startposx - 1}, {startposy + 1, startposx}, {startposy + 1, startposx + 1}};
    for (int y = 0; y < underboard.size(); y++)
    {
        for (int x = 0; x < underboard[0].size(); x++)
        {
            if (thepieces[y][x].getcolor() != color and underboard[y][x] != ' ')
            {
                collectmoveinterference(underboard, thepieces, y, x, thepieces[y][x].getcolor());
            }
        }
    }

    for (std::pair<int, int> item : dirs)
    {
        //king cannot eat protected pieces
        if (InBounds(item.first, item.second, underboard) and std::find(interferemoves.begin(), interferemoves.end(), item) == interferemoves.end()
        and (underboard[item.first][item.second] == ' ' or (underboard[item.first][item.second] != ' ' and thepieces[item.first][item.second].getcolor() != color
        and !IsPieceProtected(item.first, item.second))))
        {
            possiblemoves.emplace_back(item);
        }
        if (InBounds(item.first, item.second, underboard) and underboard[item.first][item.second] != ' '
        and thepieces[item.first][item.second].getcolor() == color)
        {
            protectingsquares.emplace_back(item);
        }
    }
    /*
 *  when it comes to checkmates, there are multiple cases
 *  checkmated cause u cannot move anywhere, cannot block,
 *  also if the piece that is within your range is protected (i.e. king cannot eat it and another piece cannot gobble it )
 *
 */

    if (kingcolor == "white")
    {
        kingcolor = "black";
    } else
    {
        kingcolor = "white";
    }
    std::pair<int, int> oppking = findking(kingcolor, underboard, thepieces);
    std::vector<std::pair<int,int>> kingoverlapmoves = grabopposingkingssquares(underboard, thepieces, oppking.first, oppking.second, color);
    for (auto items : kingoverlapmoves)
    {
        std::vector<std::pair<int,int>>::iterator it;
        it = std::find(possiblemoves.begin(), possiblemoves.end(), items);
        if (it != possiblemoves.end())
        {
            possiblemoves.erase(it);
        }
    }
}



bool Chess::King::cangobblenearking(int rowpos, int colpos, std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    // collectmoveinterference2(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, int y, int x, std::string color, std::vector<std::pair<int, int>>& addedmoves)
    std::vector<std::pair<int, int>> ourmoves;
    for (int y = 0; y < underboard.size(); y++)
    {
        for (int x = 0; x < underboard[0].size(); x++)
        {
            if (thepieces[y][x].getcolor() == color and underboard[y][x] != ' ')
            {
                collectmoveinterference2(underboard, thepieces, y, x, thepieces[y][x].getcolor(), ourmoves);
            }
        }
    }
    for (auto item : ourmoves)
    {
        if (item.first == rowpos and item.second == colpos)
        {
            return true;
        }
    }
    return false;
}




bool Chess::King::IsPieceProtected(int opppiecerowpos, int opppiececolpos) {
    for (auto protectedsq : totalprotectingsquares)
    {
        if (opppiecerowpos == protectedsq.first and opppiececolpos == protectedsq.second)
        {
            return true;
        }
    }
    return false;
}

std::pair<int, int> Chess::King::findking(std::string color, std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces) {
    for (int height = 0; height < 8; height++)
    {
        for (int width = 0; width < 8; width++)
        {
            if (underboard[height][width] == 'A' and thepieces[height][width].getcolor() == color)
            {
                std::pair<int, int> kingcoords = std::make_pair(height,width);
                return kingcoords;
            }
        }
    }
    return {};
}


void Chess::King::castleenact(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, int startposfactor, int kingcrement) {
    char temprook = underboard[endposy][startposfactor];
    Pieces tpiecer = thepieces[endposy][startposfactor];
    setblank(endposy, startposfactor, underboard, thepieces);
    char tempking = underboard[startposy][startposx];
    Pieces tpiecek = thepieces[startposy][startposx];
    setblank(startposy, startposx, underboard, thepieces);
    underboard[endposy][endposx] = tempking;
    thepieces[endposy][endposx] = tpiecek;
    underboard[endposy][kingcrement] = temprook;
    thepieces[endposy][kingcrement] = tpiecer;

}

void Chess::King::setblank(int ypos, int xpos, std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces) {
    underboard[ypos][xpos] = ' ';
    thepieces[ypos][xpos].setblank(true);
    thepieces[ypos][xpos].setcolor(" ");
    thepieces[ypos][xpos].settype(' ');
}

bool Chess::King::performCastle(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    if (color == "black" and endposy == 0 and (endposx == startposx - 2 or
    endposx == startposx + 2) and startposy == 0 and startposx == 4)
    {
         if (CastleCheck(underboard, thepieces, color))
         {
             CastleMovement(underboard, thepieces);
             return true;
         }
    } else if (color == "white" and endposy == 7 and (endposx == startposx - 2 or endposx ==
    startposx + 2) and startposy == 7 and startposx == 4)
    {
        if (CastleCheck(underboard, thepieces, color))
        {
            CastleMovement(underboard, thepieces);
            return true;
        }
    }
    return false;
}


bool Chess::King::CastleCheck(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    std::vector<std::pair<int, int>> wqsidecoords = {{startposy, startposx - 1}, {startposy, startposx - 2}, {startposy, startposx - 3}};
    std::vector<std::pair<int, int>> wkingsidecoords = {{startposy, startposx + 1}, {startposy, startposx + 2}};
    std::vector<std::pair<int, int>> bqsidecoords;
    std::vector<std::pair<int, int>> bkingsidecoords;
    bool qsidehrook = false;
    bool ksinghrook = false;
    if (InBounds(startposy, startposx - 4, underboard) and underboard[startposy][startposx - 4] == 'R'
        and thepieces[startposy][startposx - 4].getcolor() == color)
    {
        qsidehrook = true;
    }
    if (InBounds(startposy, startposx + 3, underboard) and underboard[startposy][startposx + 3] == 'R'
        and thepieces[startposy][startposx + 3].getcolor() == color)
    {
        ksinghrook = true;
    }
    if (endposx == startposx - 2)
    {
        if (!qsidehrook)
        {
            return false;
        }
        for (int xbet = startposx - 1; xbet >= 1; xbet--)
        {
            if (underboard[startposy][xbet] != ' ')
            {
                return false;
            }

        }
    }
    if (endposx == startposx + 2)
    {
        if (!ksinghrook)
        {
            return false;
        }
        for (int xbet = startposx + 1; xbet <= endposx; xbet++)
        {
            if (underboard[startposy][xbet] != ' ')
            {
                return false;
            }
        }

    }
    for (int y = 0; y < underboard.size(); y++)
    {
        for (int x = 0; x < underboard[0].size(); x++)
        {
            if (thepieces[y][x].getcolor() != color and underboard[y][x] != ' ')
            {
                collectmoveinterference(underboard, thepieces, y, x, thepieces[y][x].getcolor());
            }
        }
    }

    for (std::pair<int, int> move : interferemoves)
    {
        if (endposx == startposx - 2)
        {
            for (std::pair<int, int> item : wqsidecoords)
            {
                if (move == item)
                {
                    return false;
                }
            }

        } else if (endposx == startposx + 2)
        {
            for (std::pair<int, int> item : wkingsidecoords)
            {
                if (move == item)
                {
                    return false;
                }
            }

        }
    }
    return true;
}



bool Chess::King::collectmoveinterference(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, int y, int x, std::string color) {
    //whitew
    Pawn pawnobj(x, y, -9000, -9000);
    Rook rookobj(x, y, -9000, -9000);
    Queen queenobj(x, y, -9000, -9000);
    Bishop bishopobj(x, y, -9000, -9000);
    King kingobj(x, y, -9000, -9000);
    Knight knightobj(x, y, -9000, -9000);
    switch (underboard[y][x])
    {
        case 'P':
            pawnobj.GenerateMoves(underboard, thepieces, color);
            if (pawnobj.getstaredown())
            {
                std::cout << "Pawn stare down" << std::endl;
                return true;
            }
            this->addmoves(pawnobj.getwatchingsquares(), interferemoves);
            this->addmoves(pawnobj.protectingsquares, totalprotectingsquares);
            break;
        case 'R':
            rookobj.GenerateMoves(underboard, thepieces, color);
            if (rookobj.getstaredown())
            {
                currstarekingy = rookobj.getstaredownkingy();
                currstarekingx = rookobj.getstaredownkingx();
                std::cout << "Rook stare down" << std::endl;
                return true;
            }
            this->addmoves(rookobj.getpossiblemoves(), interferemoves);
            this->addmoves(rookobj.protectingsquares, totalprotectingsquares);
            break;
        case 'B':
            bishopobj.GenerateMoves(underboard, thepieces, color);
            if (bishopobj.getstaredown())
            {
                currstarekingy = bishopobj.getstaredownkingy();
                currstarekingx = bishopobj.getstaredownkingx();
                std::cout << "bishop stare down" << std::endl;
                return true;
            }
            this->addmoves(bishopobj.getpossiblemoves(), interferemoves);
            this->addmoves(bishopobj.protectingsquares, totalprotectingsquares);
            break;
        case 'Q':
            queenobj.GenerateMoves(underboard, thepieces, color);
            if (queenobj.getstaredown())
            {
                currstarekingy = queenobj.getstaredownkingy();
                currstarekingx = queenobj.getstaredownkingx();
                std::cout << "queen stare down" << std::endl;
                return true;
            }
            this->addmoves(queenobj.getpossiblemoves(), interferemoves);
            this->addmoves(queenobj.protectingsquares, totalprotectingsquares);
            break;
        case 'K':
            knightobj.GenerateMoves(underboard, thepieces, color);
            if (knightobj.getstaredown())
            {
                std::cout << "knight stare down" << std::endl;
                return true;
            }
            this->addmoves(knightobj.getpossiblemoves(), interferemoves);
            this->addmoves(knightobj.protectingsquares, totalprotectingsquares);
            break;
        default:
            break;
    }
    return false;
}


void Chess::King::collectmoveinterference2(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, int y, int x, std::string color, std::vector<std::pair<int, int>>& addedmoves) {
    Pawn pawnobj(x, y, -9000, -9000);
    Rook rookobj(x, y, -9000, -9000);
    Queen queenobj(x, y, -9000, -9000);
    Bishop bishopobj(x, y, -9000, -9000);
    King kingobj(x, y, -9000, -9000);
    Knight knightobj(x, y, -9000, -9000);
    switch (underboard[y][x])
    {
        case 'P':
            pawnobj.GenerateMoves(underboard, thepieces, color);
            break;
        case 'R':
            this->addmoves(rookobj.getpossiblemoves(), addedmoves);
            break;
        case 'B':
            bishopobj.GenerateMoves(underboard, thepieces, color);
            this->addmoves(bishopobj.getpossiblemoves(), addedmoves);
            break;
        case 'Q':
            queenobj.GenerateMoves(underboard, thepieces, color);
            this->addmoves(queenobj.getpossiblemoves(), addedmoves);
            break;
        case 'K':
            knightobj.GenerateMoves(underboard, thepieces, color);
            this->addmoves(knightobj.getpossiblemoves(),  addedmoves);
            break;
        default:
            break;
    }
}

void Chess::King::addmoves(std::vector<std::pair<int, int>> src, std::vector<std::pair<int, int>> &destination) {
    for (auto item : src)
    {
        destination.emplace_back(item);
    }
}

std::vector<std::pair<int,int>> Chess::King::getpossiblemoves() {
    return possiblemoves;
}


std::vector<std::pair<int,int>> Chess::King::grabopposingkingssquares(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, int y, int x, std::string color) {
    std::vector<std::pair<int, int>> dirs = {{startposy - 1, startposx - 1}, {startposy - 1, startposx}, {startposy - 1, startposx + 1}, {startposy, startposx - 1}, {startposy, startposx + 1},
                                             {startposy + 1, startposx - 1}, {startposy + 1, startposx}, {startposy + 1, startposx + 1}};
    std::vector<std::pair<int, int>> opponentdirs = {{y - 1, x - 1}, {y - 1, x}, {y - 1, x + 1}, {y, x - 1}, {y, x+ 1},
                                                {y + 1, x- 1}, {y + 1, x}, {y + 1, x + 1}};
    std::vector<std::pair<int,int>> overlappingmoves;
    for (auto ourking : dirs) {
        for (auto opponentking: opponentdirs) {
            if (InBounds(ourking.first, ourking.second, underboard) and
                InBounds(opponentking.first, opponentking.second, underboard) and ourking == opponentking) {
                overlappingmoves.push_back({ourking.first, ourking.second});
            }
        }
    }
    return overlappingmoves;
}


bool Chess::King::determinecheck(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    for (int y = 0; y < underboard.size(); y++)
    {
        for (int x = 0; x < underboard[0].size(); x++)
        {
            if (thepieces[y][x].getcolor() != color and underboard[y][x] != ' ' and underboard[y][x] != 'A')
            {
                if (collectmoveinterference(underboard, thepieces, y, x, thepieces[y][x].getcolor())) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<int> Chess::King::grabstaredown(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    for (int y = 0; y < underboard.size(); y++)
    {
        for (int x = 0; x < underboard[0].size(); x++)
        {
            if (thepieces[y][x].getcolor() != color and underboard[y][x] != ' ' and underboard[y][x] != 'A')
            {
                if (collectmoveinterference(underboard, thepieces, y, x, thepieces[y][x].getcolor())) {
                    std::vector<int> staredownpiece = {y, x, underboard[y][x]};
                    return staredownpiece;
                }
            }
        }
    }
    return {};
}

/*
 *  when it comes to checkmates, there are multiple cases
 *  checkmated cause u cannot move anywhere, cannot block,
 *  also if the piece that is within your range is protected (i.e. king cannot eat it and another piece cannot gobble it )
 *
 */
bool Chess::King::cannotblock(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    std::vector<int> piecechecking;
    char piecetype;
    piecechecking = grabstaredown(underboard, thepieces, color);
    std::vector<std::pair<int, int>> dirs = {{startposy - 1, startposx - 1}, {startposy - 1, startposx}, {startposy - 1, startposx + 1}, {startposy, startposx - 1}, {startposy, startposx + 1},
                                             {startposy + 1, startposx - 1}, {startposy + 1, startposx}, {startposy + 1, startposx + 1}};
    //deal with protected pieces here;
    //have to deal with when the piece that is checking is in the range of the king
    //from one of the directions
    //if the piece is in the range (standing on one of the king's 8 tiles)
    //
    if (piecechecking.size() == 0)
    {
        return false;
    }
    for (auto item : dirs)
    {
        if (item.first >= 0 and item.first < underboard.size()
        and item.second >= 0 and item.second < underboard[0].size())
        {
            if (currstarekingy != -9000 and currstarekingx != -9000 and
            currstarekingy == item.first and currstarekingx == item.second)
            {
                for (int y = 0; y < underboard.size(); y++)
                {
                    for (int x = 0; x < underboard[0].size(); x++)
                    {
                        if (thepieces[y][x].getcolor() == color and underboard[y][x] != ' ' and underboard[y][x] != 'A')
                        {
                            collectmoveinterference2(underboard, thepieces, y, x, thepieces[y][x].getcolor(), interferemoves2);
                        }
                    }
                }
                for (auto intmoves : interferemoves2)
                {
                    if (intmoves.first == currstarekingy and intmoves.second == currstarekingx)
                    {
                        return false;
                    }
                }
            }
        }
    }

    //generate the



    return true;
}


bool Chess::King::determinecheckmate(std::vector<std::vector<char>>& underboard, std::vector<std::vector<Pieces>>& thepieces, std::string color) {
    GenerateMoves(underboard, thepieces, color);
    if (determinecheck(underboard, thepieces, color) and possiblemoves.size() == 0
    and cannotblock(underboard, thepieces, color))
    {
        return true;
    } else
    {
        return false;
    }

}

void Chess::King::clearpossiblemoves() {
    possiblemoves = {};
}


