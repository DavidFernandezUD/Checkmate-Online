#include "User.h"

User::User() {
    // Constructor
}

void User::setUsername(const std::string& name) {
    username = name;
}

std::string User::getUsername() const {
    return username;
}
