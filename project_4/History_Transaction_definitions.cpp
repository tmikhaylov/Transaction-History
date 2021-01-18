#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
        unsigned int month_date,  unsigned int year_date,
        bool buy_sell_trans,  unsigned int number_shares,
        double trans_amount ) {
	symbol = ticker_symbol;
	day = day_date;
	month = month_date;
	year = year_date;
	if (buy_sell_trans == true) {
		trans_type = "Buy";
	} else if (buy_sell_trans == false) {
		trans_type = "Sell";			//cgl = trans_amount - (number_shares * acb_per_share);
	}
	shares = number_shares;
	amount = trans_amount;
	trans_id = assigned_trans_id;
	++assigned_trans_id;
	acb = 0;						//used to be += amount
	acb_per_share = 0;
	share_balance = 0;
	cgl = 0;
	p_next = nullptr;

}

// Destructor
// TASK
//
Transaction::~Transaction() {

}

// TASK
// Overloaded < operator.
//
bool Transaction::operator<( Transaction const &other ) {
	if (this->get_year() < other.get_year()) {
		return true;
	} else if (this->get_year() == other.get_year()) {
		if (this->get_month() < other.get_month()) {
			return true;
		} else if (this->get_month() == other.get_month()) {
			if (this->get_day() < other.get_day()) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History::History() {
	p_head = nullptr;
}


// Destructor
// TASK
//
History::~History() {
	Transaction *p_old_head{nullptr};
	while (get_p_head() != nullptr) {
		p_old_head = get_p_head();

		p_head = p_head->get_next();
		delete p_old_head;
		p_old_head = nullptr;
	}
	p_old_head = nullptr;
}

// TASK
// read_history(...): Read the transaction history from file.
//
void History::read_history() {
	ece150::open_file();
	History file{};
	if (ece150::next_trans_entry()) {
		Transaction *entry{new Transaction{ece150::get_trans_symbol(),
			ece150::get_trans_day(), ece150::get_trans_month(),
			ece150::get_trans_year(), ece150::get_trans_type(),
			ece150::get_trans_shares(), ece150::get_trans_amount()}};
		this->insert(entry);
//		entry->print();
	}
	while (ece150::next_trans_entry()) {
		Transaction *entry{new Transaction{ece150::get_trans_symbol(),
			ece150::get_trans_day(), ece150::get_trans_month(),
			ece150::get_trans_year(), ece150::get_trans_type(),
			ece150::get_trans_shares(), ece150::get_trans_amount()}};
		this->insert(entry);
//		entry->print();
	}
	ece150::close_file();

}

// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans) {
	Transaction *p_tmp{get_p_head()};
	if (get_p_head() == nullptr) {
		p_head = p_new_trans;
	} else {
		while (p_tmp->get_next() != nullptr) {
			p_tmp = p_tmp->get_next();
		}
		p_tmp->set_next(p_new_trans);
	}
	p_tmp = nullptr;
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History::sort_by_date() {
	if (this->get_p_head() == nullptr || this->get_p_head()->get_next() == nullptr) { //no need to sort
		return;
	} else { //there are at least two nodes
		Transaction *p_tmp_1{this->get_p_head()}; 	//pointer to walk through the initial list
		Transaction *p_tmp_sorted{nullptr};			//pointer to walk through the sorted list
		Transaction *p_head_sorted{nullptr};		//head pointer for the sorted list
		//break off the first node
		this->p_head = this->get_p_head()->get_next();
		p_tmp_1->set_next(nullptr);
		//put the first node into the sorted list
		p_head_sorted = p_tmp_1;
		//now the sorted list has exactly one node
		while (this->get_p_head() != nullptr) {
			//detaching the node
			p_tmp_1 = this->get_p_head();
			this->p_head = this->get_p_head()->get_next();
			p_tmp_1->set_next(nullptr);
			if ( ( *p_tmp_1 < *p_head_sorted ) ||
					( (p_tmp_1->get_year() == p_head_sorted->get_year()) &&
							(p_tmp_1->get_month() == p_head_sorted->get_month()) &&
							(p_tmp_1->get_day() == p_head_sorted->get_day()) &&
							(p_tmp_1->get_trans_id() < p_head_sorted->get_trans_id()) )) { //node goes front
				p_tmp_1->set_next(p_head_sorted);
				p_head_sorted = p_tmp_1;
			} else { //the node goes elsewhere in the list
				p_tmp_sorted = p_head_sorted;
				while ( (p_tmp_sorted->get_next() != nullptr) &&
						!( (*p_tmp_1 < *(p_tmp_sorted->get_next())) ||
						((p_tmp_1->get_year() == (p_tmp_sorted->get_next())->get_year()) &&
								(p_tmp_1->get_month() == (p_tmp_sorted->get_next())->get_month()) &&
								(p_tmp_1->get_day() == (p_tmp_sorted->get_next())->get_day()) &&
								(p_tmp_1->get_trans_id() < (p_tmp_sorted->get_next())->get_trans_id()) ) )) {
					p_tmp_sorted = p_tmp_sorted->get_next();
				}
				p_tmp_1->set_next(p_tmp_sorted->get_next());
				p_tmp_sorted->set_next(p_tmp_1);
			}
		}
		this->p_head = p_head_sorted;
		p_tmp_1 = nullptr;
		p_tmp_sorted = nullptr;
		p_head_sorted = nullptr;
	}
}


// TASK
// update_acb_cgl(): Updates the ACB and CGL values.
//
void History::update_acb_cgl(){	//works fine?!
	unsigned int shares{0};
	double acb_per_share{0};
	double acb{0};
	double cgl{0};
	Transaction *p_tmp{this->get_p_head()};
	while (p_tmp != nullptr) { //might need to change the condition????
		if (p_tmp->get_trans_type()) {
			acb += p_tmp->get_amount();
			shares += p_tmp->get_shares();
			acb_per_share = acb/shares;
			cgl = 0;
		} else {
			shares -= p_tmp->get_shares();
			acb = acb - (p_tmp->get_shares()*acb_per_share);
			cgl = p_tmp->get_amount() - (p_tmp->get_shares()*acb_per_share);
		}
		p_tmp->set_acb(acb);
		p_tmp->set_acb_per_share(acb_per_share);
		p_tmp->set_share_balance(shares);
		p_tmp->set_cgl(cgl);

		p_tmp = p_tmp->get_next();
	}
	p_tmp = nullptr;

}


// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year) {
	History::update_acb_cgl();
	Transaction *p_tmp{get_p_head()};
	double cgl{0};
	this->sort_by_date();
	while (p_tmp != nullptr) {
		if (p_tmp->get_year() == year) {
			cgl += p_tmp->get_cgl();
		}
		p_tmp = p_tmp->get_next();
	}
	p_tmp = nullptr;

	return cgl;
}



// TASK
// print() Print the transaction history.
//
void History::print() {
	std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
	Transaction *p_tmp{get_p_head()};
	while (p_tmp != nullptr) {
		p_tmp->print();
		p_tmp = p_tmp->get_next();
	}
	std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
	p_tmp = nullptr;
}



// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }

//WORKING SORTING ALGORITHM???!!!
/*
void History::sort_by_date() {
	if (this->get_p_head() == nullptr || this->get_p_head()->get_next() == nullptr) { //no need to sort
		return;
	} else { //there are at least two nodes
		Transaction *p_tmp_1{this->get_p_head()}; 	//pointer to walk through the initial list
		Transaction *p_tmp_sorted{nullptr};			//pointer to walk through the sorted list
		Transaction *p_head_sorted{nullptr};		//head pointer for the sorted list
		//break off the first node
		this->p_head = this->get_p_head()->get_next();
		p_tmp_1->set_next(nullptr);
		//put the first node into the sorted list
		p_head_sorted = p_tmp_1;
		//now the sorted list has exactly one node
		while (this->get_p_head() != nullptr) {
			//detaching the node
			p_tmp_1 = this->get_p_head();
			this->p_head = this->get_p_head()->get_next();
			p_tmp_1->set_next(nullptr);
			if ( ( *p_tmp_1 < *p_head_sorted ) ||
					( (p_tmp_1->get_year() == p_head_sorted->get_year()) &&
							(p_tmp_1->get_month() == p_head_sorted->get_month()) &&
							(p_tmp_1->get_day() == p_head_sorted->get_day()) &&
							(p_tmp_1->get_trans_id() < p_head_sorted->get_trans_id()) )) { //node goes front
				p_tmp_1->set_next(p_head_sorted);
				p_head_sorted = p_tmp_1;
			} else { //the node goes elsewhere in the list
				p_tmp_sorted = p_head_sorted;
				while ( (p_tmp_sorted->get_next() != nullptr) &&
						!( (*p_tmp_1 < *(p_tmp_sorted->get_next())) ||
						((p_tmp_1->get_year() == (p_tmp_sorted->get_next())->get_year()) &&
								(p_tmp_1->get_month() == (p_tmp_sorted->get_next())->get_month()) &&
								(p_tmp_1->get_day() == (p_tmp_sorted->get_next())->get_day()) &&
								(p_tmp_1->get_trans_id() < (p_tmp_sorted->get_next())->get_trans_id()) ) )) {
					p_tmp_sorted = p_tmp_sorted->get_next();
				}
				p_tmp_1->set_next(p_tmp_sorted->get_next());
				p_tmp_sorted->set_next(p_tmp_1);
			}
		}
		this->p_head = p_head_sorted;
	}
}
*/



//OLD SORTING ALGORITHM:
/*
void History::sort_by_date() {
	if (this->get_p_head() == nullptr || this->get_p_head()->get_next() == nullptr) { //the list has at least 2 nodes
		return;
	} else {
		Transaction *p_tmp_initial{get_p_head()};	//temporary pointer for the initial linked list
		Transaction *p_head_sorted{nullptr};//head pointer for the sorted linked list
		Transaction *p_tmp_sorted{nullptr}; //temporary pointer for the sorted linked list
		this->p_head = this->p_head->get_next(); //p_head now points onto second node
		p_tmp_initial->set_next(nullptr); //the first node's p_next is now nullptr (it's cut off) --> the first node has now been cut off from the original linked list
		p_head_sorted = p_tmp_initial; //p_head_sorted now points onto the first node of the original linked list
		while (this->get_p_head() != nullptr) {
			p_tmp_initial = this->get_p_head(); //p_tmp_initial now points onto the new first (old second) node of the original linked list
			this->p_head = this->get_p_head()->get_next(); //p_head now points onto the third node of the original linked list
			p_tmp_initial->set_next(nullptr); //the new first (old second) node is now cut off from the original linked list
			//at this point:
			//p_head points onto the third node of the original linked list
			//p_tmp_initial points onto the second node of the initial linked list
			//p_head_sorted points onto the first node of the initial linked list
			//p_tmp_sorted is still a nullptr

			//We now consider the following two cases:
			//case 1: transferred node (stored at p_tmp_initial) goes to the front of the list
			//case 2: transferred node (stored at p_tmp_initial) goes elsewhere in the list
			if ( (*p_tmp_initial < *p_head_sorted) ||
					(( (p_tmp_initial->get_year() == p_head_sorted->get_year() &&
							p_tmp_initial->get_month() == p_head_sorted->get_month()) &&
							p_tmp_initial->get_day() == p_head_sorted->get_day()) &&
							(p_tmp_initial->get_trans_id() < p_head_sorted->get_trans_id()) )) { 											//case 1
				p_tmp_initial->set_next(p_head_sorted);
				p_head_sorted = p_tmp_initial;
			} else if ( ((p_tmp_initial->get_year() == p_head_sorted->get_year()) &&		//checking for
					(p_tmp_initial->get_month() == p_head_sorted->get_month())) &&		//equal dates
					(p_tmp_initial->get_day() == p_head_sorted->get_day())) {
				if (p_tmp_initial->get_trans_id() < p_head_sorted->get_trans_id()) { 		//case 1
					p_tmp_initial->set_next(p_head_sorted);
					p_head_sorted = p_tmp_initial;
				} else {																	//case 2
					p_tmp_sorted = p_head_sorted;
					while ( ( !(*p_tmp_initial < *(p_tmp_sorted->get_next())) ||
							 !( (p_tmp_initial->get_year() == p_tmp_sorted->get_year() ) &&
							(p_tmp_initial->get_month() == p_tmp_sorted->get_month()) &&
							(p_tmp_initial->get_day() == p_tmp_sorted->get_day()) &&
							(p_tmp_initial->get_trans_id() < p_tmp_sorted->get_trans_id())) ) &&
							(p_tmp_sorted->get_next() != nullptr) ){
						p_tmp_sorted = p_tmp_sorted->get_next();
					}
					p_tmp_initial->set_next(p_tmp_sorted->get_next());
					p_tmp_sorted->set_next(p_tmp_initial);
				}
			} else {	//case 2
				p_tmp_sorted = p_head_sorted;
				while (  !( (*p_tmp_initial < *(p_tmp_sorted->get_next())) ||
						(( (p_tmp_initial->get_year() == p_tmp_sorted->get_next()->get_year() &&
								p_tmp_initial->get_month() == p_tmp_sorted->get_next()->get_month()) &&
								p_tmp_initial->get_day() == p_tmp_sorted->get_next()->get_day()) &&
								(p_tmp_initial->get_trans_id() < p_tmp_sorted->get_next()->get_trans_id()) ) ) &&
						(p_tmp_sorted->get_next() != nullptr) ) {
					p_tmp_sorted = p_tmp_sorted->get_next();
				}
				p_tmp_initial->set_next(p_tmp_sorted->get_next());
				p_tmp_sorted->set_next(p_tmp_initial);
			}
		}
		this->p_head = p_head_sorted;
		p_tmp_initial = nullptr;
		p_tmp_sorted = nullptr;
		p_head_sorted = nullptr;
	}
}
*/
