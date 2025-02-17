#pragma once
#include <iostream>

enum class Ranks { Flag, Spy, Scout, Miner, Sergeant, Lieutenant, Captain, Major, Colonel, General, Marshal, Bomb, None };
enum class Players { Red, Blue, None };

// Function to convert Ranks enum to string
inline std::string toString(const Ranks& rank) {
    switch(rank) {
        case Ranks::Flag: return "Flag";
        case Ranks::Spy: return "Spy";
        case Ranks::Scout: return "Scout";
        case Ranks::Miner: return "Miner";
        case Ranks::Sergeant: return "Sergeant";
        case Ranks::Lieutenant: return "Lieutenant";
        case Ranks::Captain: return "Captain";
        case Ranks::Major: return "Major";
        case Ranks::Colonel: return "Colonel";
        case Ranks::General: return "General";
        case Ranks::Marshal: return "Marshal";
        case Ranks::Bomb: return "Bomb";
        case Ranks::None: return "None";
        default: return "Unknown Rank";
    }
}

// Function to convert Players enum to string
inline std::string toString(const Players& player) {
    switch(player) {
        case Players::Red: return "Red";
        case Players::Blue: return "Blue";
        case Players::None: return "None";
        default: return "Unknown Player";
    }
}

inline std::ostream& operator<<(std::ostream& os, const Ranks& rank) {
    switch(rank) {
        case Ranks::Flag: os << "Flag"; break;
        case Ranks::Spy: os << "Spy"; break;
        case Ranks::Scout: os << "Scout"; break;
        case Ranks::Miner: os << "Miner"; break;
        case Ranks::Sergeant: os << "Sergeant"; break;
        case Ranks::Lieutenant: os << "Lieutenant"; break;
        case Ranks::Captain: os << "Captain"; break;
        case Ranks::Major: os << "Major"; break;
        case Ranks::Colonel: os << "Colonel"; break;
        case Ranks::General: os << "General"; break;
        case Ranks::Marshal: os << "Marshal"; break;
        case Ranks::Bomb: os << "Bomb"; break;
        default: os.setstate(std::ios_base::failbit);
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Players& player) {
    switch(player) {
        case Players::Red: os << "Red"; break;
        case Players::Blue: os << "Blue"; break;
        default: os.setstate(std::ios_base::failbit);
    }
    return os;
}


