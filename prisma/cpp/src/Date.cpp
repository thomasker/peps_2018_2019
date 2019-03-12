
#include "Date.h"

namespace Outils
{

	date::date(int jour, int mois, int anne, jourSemaine jourSemaine) {
		_jour = jour;
		_mois = mois;
		_anne = anne;
		_jourSemaine = jourSemaine;
	}

	date::date(const date& DateToCopy) {
		_jour = DateToCopy._jour;
		_mois = DateToCopy._mois;
		_anne = DateToCopy._anne;
		_jourSemaine = DateToCopy._jourSemaine;
	}

	void date::addDay() {
		_jour++;
		switch (_jourSemaine)
		{
		case Lundi:
			_jourSemaine = Mardi;
			break;
		case Mardi:
			_jourSemaine = Mercredi;
			break;
		case Mercredi:
			_jourSemaine = Jeudi;
			break;
		case Jeudi:
			_jourSemaine = Vendredi;
			break;
		case Vendredi:
			_jourSemaine = Samedi;
			break;
		case Samedi:
			_jourSemaine = Dimanche;
			break;
		case Dimanche:
			_jourSemaine = Lundi;
			break;
		default:
			break;
		}
		//fevrier
		if (_mois == 2 && (_jour > 29 || (_jour > 28 && _anne % 4 != 0))) {
			_jour = 1;
			_mois += 1;
		}
		if (_jour > 31 || _jour > 30 && ((_mois < 8 && _mois % 2 == 0) || (_mois >= 8 && _mois % 2 == 1))) {
			_jour = 1;
			_mois += 1;
		}
		if (_mois > 12) {
			_mois = 1;
			_anne += 1;
		}
	}

	bool date::isWeekend() {
		return _jourSemaine == Samedi || _jourSemaine == Dimanche;
	}

	bool date::isClosedDay() {
		/*if (_jourferiee ==NULL) {*/
		date jourferiee[48] = {
		date(01, 01, 2002),	// Jour de l'an
		date(29, 03, 2002),	// Vendredi Saint
		date(01, 04, 2002),	// Le Lundi de Pâques 
		date(01, 05, 2002),	// Fête du Travail
		date(24, 12, 2002),	// Veille du Jour de Noël(Fermeture à 14:05)
		date(25, 12, 2002), // Jour de Noël
		date(26, 12, 2002), // Lendemain de jour de Noël
		date(31, 12, 2002), // Veille du Jour de l'an (Fermeture à 14:05)

		date(01, 01, 2003),	// Jour de l'an
		date(18, 04, 2003),	// Vendredi Saint
		date(21, 04, 2003),	// Le Lundi de Pâques 
		date(01, 05, 2003),	// Fête du Travail
		date(24, 12, 2003),	// Veille du Jour de Noël(Fermeture à 14:05)
		date(25, 12, 2003), // Jour de Noël
		date(26, 12, 2003), // Lendemain de jour de Noël
		date(31, 12, 2003), // Veille du Jour de l'an (Fermeture à 14:05)

		date(01, 01, 2004),	// Jour de l'an
		date(9, 04, 2004),	// Vendredi Saint
		date(12, 04, 2004),	// Le Lundi de Pâques 
		date(01, 05, 2004),	// Fête du Travail
		date(24, 12, 2004),	// Veille du Jour de Noël(Fermeture à 14:05)
		date(25, 12, 2004), // Jour de Noël
		date(26, 12, 2004), // Lendemain de jour de Noël
		date(31, 12, 2004), // Veille du Jour de l'an (Fermeture à 14:05)

		date(01, 01, 2005),	// Jour de l'an
		date(25, 03, 2005),	// Vendredi Saint
		date(28, 03, 2005),	// Le Lundi de Pâques 
		date(01, 05, 2005),	// Fête du Travail
		date(24, 12, 2005),	// Veille du Jour de Noël(Fermeture à 14:05)
		date(25, 12, 2005), // Jour de Noël
		date(26, 12, 2005), // Lendemain de jour de Noël
		date(31, 12, 2005), // Veille du Jour de l'an (Fermeture à 14:05)

		date(01, 01, 2006),	// Jour de l'an
		date(14, 04, 2006),	// Vendredi Saint
		date(17, 04, 2006),	// Le Lundi de Pâques 
		date(01, 05, 2006),	// Fête du Travail
		date(24, 12, 2006),	// Veille du Jour de Noël(Fermeture à 14:05)
		date(25, 12, 2006), // Jour de Noël
		date(26, 12, 2006), // Lendemain de jour de Noël
		date(31, 12, 2006), // Veille du Jour de l'an (Fermeture à 14:05)

		date(01, 01, 2007),	// Jour de l'an
		date(06, 04, 2007),	// Vendredi Saint
		date(9, 04, 2007),	// Le Lundi de Pâques 
		date(01, 05, 2007),	// Fête du Travail
		date(24, 12, 2007),	// Veille du Jour de Noël(Fermeture à 14:05)
		date(25, 12, 2007), // Jour de Noël
		date(26, 12, 2007), // Lendemain de jour de Noël
		date(31, 12, 2007) // Veille du Jour de l'an (Fermeture à 14:05)

		};
		/*_jourferiee = jourferiee;
	}*/
		for (int i = 0; i < 48; i++) {
			if (areEqual(jourferiee[i], *this)) {
				return true;
			}
		}
		return false;
		/*return std::find(_jourferiee.begin(), _jourferiee.end(), *this) != _jourferiee.end();*/
	}

	int date::getNbDay(const date toDate) {
		date date = *this;
		int count = 0;
		while (date < toDate) {
			date.addDay();
			count++;
		}
		return count;
	};
	bool date::areEqual(const date& a, const date& b) {
		return (b._jour == a._jour && b._mois == a._mois && b._anne == a._anne);
	}

	bool date::operator==(const date& a)const {
		return (_jour == a._jour && _mois == a._mois && _anne == a._anne);
	}

	bool date::operator<(const date& a)const {
		return (_anne < a._anne ||(_anne == a._anne && _mois < a._mois) || (_anne == a._anne && _mois == a._mois && _jour < a._jour) );
	}

	bool date::operator>(const date& a)const {
		return (_anne > a._anne || (_anne == a._anne && _mois > a._mois) || (_anne == a._anne && _mois == a._mois && _jour > a._jour) );
	}

}