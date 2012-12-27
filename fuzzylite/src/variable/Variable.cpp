/*
 * Variable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/variable/Variable.h"

#include "fl/term/Term.h"

#include "fl/operator/Operator.h"

#include <algorithm>
#include <sstream>

namespace fl {

    Variable::Variable(const std::string& name, scalar minimum, scalar maximum)
    : _name(name), _minimum(minimum), _maximum(maximum) {
    }

    Variable::Variable(const Variable& copy) {
        for (int i = 0; i < copy.numberOfTerms(); ++i) {
            addTerm(copy.getTerm(i)->copy());
        }
    }

    Variable::~Variable() {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms[i];
        }
    }

    void Variable::configure(Configuration* config) {
        //do nothing
    }

    void Variable::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Variable::getName() const {
        return this->_name;
    }

    //It is assumed the terms are inserted in ascending order

    void Variable::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Variable::getMinimum() const {
        return this->_minimum;
    }

    void Variable::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Variable::getMaximum() const {
        return this->_maximum;
    }

    std::string Variable::fuzzify(scalar x) const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << _terms[i]->membership(x) << "/" << _terms[i]->getName();
            if (i < _terms.size() - 1)
                ss << ", ";
        }
        return ss.str();
    }

    Term* Variable::highestMembership(scalar x, scalar* yhighest) const {
        Term* result = NULL;
        scalar ymax = 0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            scalar y = _terms[i]->membership(x);
            if (fl::Op::IsGt(y, ymax)) {
                ymax = y;
                result = _terms[i];
            }
        }
        if (yhighest) *yhighest = ymax;
        return result;
    }

    std::string Variable::toString() const {
        std::ostringstream ss;
        ss << getName() << " [";
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << _terms[i]->toString();
            if (i < _terms.size() - 1) ss << ", ";
        }
        ss << "]";
        return ss.str();
    }

    /**
     * Operations for datatype _terms
     */

    void Variable::sort() {
        SortByCoG criterion;
        criterion.minimum = _minimum;
        criterion.maximum = _maximum;
        std::sort(_terms.begin(), _terms.end(), criterion);
    }

    void Variable::addTerm(Term* term) {
        this->_terms.push_back(term);
    }

    void Variable::insertTerm(Term* term, int index) {
        this->_terms.insert(this->_terms.begin() + index, term);
    }

    Term* Variable::getTerm(int index) const {
        return this->_terms[index];
    }

    Term* Variable::getTerm(const std::string& name) const {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            if (_terms[i]->getName() == name) {
                return _terms[i];
            }
        }
        return NULL;
    }

    bool Variable::hasTerm(const std::string& name) const {
        return getTerm(name) != NULL;
    }

    Term* Variable::removeTerm(int index) {
        Term* result = this->_terms[index];
        this->_terms.erase(this->_terms.begin() + index);
        return result;
    }

    int Variable::numberOfTerms() const {
        return this->_terms.size();
    }

    const std::vector<Term*>& Variable::terms() const {
        return this->_terms;
    }

}

