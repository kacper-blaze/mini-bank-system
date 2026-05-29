#include "Customer.hpp"
#include <iostream>
#include <utility>

Customer::Customer(std::string firstName, std::string lastName, std::string phone,
    std::string email) : firstName{std::move(firstName)}, lastName{std::move(lastName)},
    phoneNumber{std::move(phone)}, emailAddress{std::move(email)} {}

std::string Customer::getFullName() const {
    return firstName + " " + lastName;
}

std::string Customer::getPhone() const {
    return phoneNumber;
}

std::string Customer::getEmail() const {
    return emailAddress;
}

void Customer::setEmail(const std::string &newEmail) {
    emailAddress = newEmail;
}

void Customer::setPhone(const std::string &newPhone) {
    phoneNumber = newPhone;
}

void Customer::setFirstName(const std::string &newFirstName) {
    firstName = newFirstName;
}

void Customer::setLastName(const std::string &newLastName) {
    lastName = newLastName;
}

void Customer::display() const {
    std::cout << "OWNER DETAILS:\n";
    std::cout << "  Name:         " << getFullName() << "\n";
    std::cout << "  Phone:        " << phoneNumber << "\n";
    std::cout << "  Email:        " << emailAddress << "\n";
}
