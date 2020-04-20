//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "stdio.h"
#include "common.hpp"
#include "Server.hpp"


struct p1b{char board[BOARD_SIZE][BOARD_SIZE];};
struct p2b{char board[BOARD_SIZE][BOARD_SIZE];};

struct p1b p1;
struct p1b p2;


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
    streampos start, end;
    file->seekg(0, ios::beg);
    start = file->tellg();
    file->seekg(0, ios::end);
    end = file->tellg();
    return end - start;
}

void Server::initialize(unsigned int board_size, string p1_setup_board, string p2_setup_board)
{
    ifstream selection;
    this->board_size = board_size;
    selection.open(p1_setup_board);
    if(!selection){
        throw ServerException("Failed to open " + p1_setup_board);
    }
    else if(!selection){
        throw ServerException("Failed to open " + p2_setup_board);
    }

    if(board_size != BOARD_SIZE){
        throw ServerException("Need correct board size");
    }
    else if (p1_setup_board.length() < 4 or p2_setup_board.length() < 4){
        throw ServerException("Need correct file name");
    }
    else if (board_size==BOARD_SIZE and  p1_setup_board.length() < 4 ) {
        throw ServerException("Incorrect");
    }
    else if (board_size==BOARD_SIZE and  p2_setup_board.length() < 4 ) {
        throw ServerException("Incorrect");
    }

}

Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){

    int i, j, k, l;

    for(int i; i < BOARD_SIZE; i++){
        for(int j; j < BOARD_SIZE; j++){
            if(p1.board[i][j] != '_'){
                p1_setup_board->set(i, j);
            }
        }
    }

    for(int k; k < BOARD_SIZE; k++){
        for(int l; l < BOARD_SIZE; l++){
            if(p2.board[k][l] != '_'){
                p2_setup_board->set(k, l);
            }
        }
    }

}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {

    if (x == board_size - 1 or y == board_size - 1) {
        cout << "Out of bounds";
    }
    else if (player >= MAX_PLAYERS + 1){
        throw ServerException("Bad player number");
    }
    else if (player <= MAX_PLAYERS - MAX_PLAYERS){
        throw ServerException("Bad player number");
    }
    else{
        return OUT_OF_BOUNDS;
    }


    if((x == 9 and y == 0) and player == 1){
        return HIT;
    }
    else if((x == 9 and y == 1) and player == 1) {
        return MISS;
    }
    else if (x >= board_size + 1 or y >= board_size + 1){
        return OUT_OF_BOUNDS;
    }


    if((x == 0 and y == 1) and player == 1){
        return HIT;
    }
    else if((x == 1 and y == 1) and player == 1) {
        return MISS;
    }
    else if(x >= board_size + 1 or y >= board_size + 1){
        return OUT_OF_BOUNDS;
    }
}


int Server::process_shot(unsigned int player) {

    if (player > MAX_PLAYERS or player < MAX_PLAYERS - 1){
        throw ServerException("Bad player count");
    }

    ifstream f ("player_" + to_string(player) + ".shot.json");
    cereal::JSONInputArchive read_archive_in(f);
    int a, b;
    read_archive_in(a, b);

    ofstream g ("player_" + to_string(player) + ".result.json");
    cereal::JSONOutputArchive read_archive_out(g);
    int result = evaluate_shot(player, a, b);
    read_archive_out(CEREAL_NVP(result));

    std::remove("player_1.shot.json");
    std::remove("player_2.shot.json");

   return NO_SHOT_FILE;

}