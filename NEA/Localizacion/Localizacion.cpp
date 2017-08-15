/***************************************************************************
****************************************************************************
****************************   PseudoD    **********************************
***** Creado por Alejandro Linarez Rangel El 14 de septiembre de 2014. *****
*****                                                                  *****
***** Página oficial de PseudoD en:                                    *****
*****     http://pseudod.sourceforge.net/                              *****
*****                                                                  *****
****************************************************************************
Copyright 2016 Alejandro Linarez Rangel

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**************************************************************************/

namespace Localizacion
{
	using namespace pseudod::Utilidades;

	PseudoFijarLocalizacion::PseudoFijarLocalizacion(PDCadena nloc)
		: locale(nloc), var(false)
	{
		this->FijarClave("Fijar", "Localizacion");
	}

	PseudoFijarLocalizacion::~PseudoFijarLocalizacion(void) {}

	void PseudoFijarLocalizacion::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::locale tg;

		if(this->var && (this->locale != "*"))
		{
			tg = std::locale((std::string) data->ObtenerVariable(this->locale));
		}
		else if(this->locale != "*")
		{
			tg = std::locale((std::string) this->locale);
		}
		else
		{
			tg = std::locale();
		}

		std::locale::global(tg);

		std::cout.imbue(std::locale());
		std::wcout.imbue(std::locale());
		std::cin.imbue(std::locale());
		std::wcin.imbue(std::locale());
		std::cerr.imbue(std::locale());
		std::wcerr.imbue(std::locale());
	}

	void PseudoFijarLocalizacion::LeerParametros(pseudod::Tokenizador& in)
	{
		pseudod::Token tok;

		if(!(in >> tok))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale': EOF inesperado"
			);
		}

		if(!Tokens::EsIdentificador(tok))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale': 'locale' no es un identificador"
			);
		}

		this->locale = Tokens::ObtenerValor(tok);
		this->var = true;
	}

	PseudoReestablecerLocalizacion::PseudoReestablecerLocalizacion(void)
	{
		this->FijarClave("Reestablecer", "Localizacion");
	}

	PseudoReestablecerLocalizacion::~PseudoReestablecerLocalizacion(void) {}

	void PseudoReestablecerLocalizacion::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::locale::global(std::locale::classic());

		std::cout.imbue(std::locale());
		std::wcout.imbue(std::locale());
		std::cin.imbue(std::locale());
		std::wcin.imbue(std::locale());
		std::cerr.imbue(std::locale());
		std::wcerr.imbue(std::locale());
	}

	void PseudoReestablecerLocalizacion::LeerParametros(pseudod::Tokenizador& in)
	{}

	PseudoConvertirDecimal::PseudoConvertirDecimal(PDCadena nloc, PDCadena var)
		: locale(nloc), var(false), vard(var)
	{
		this->FijarClave("ConvertirDecimalALocal", "Localizacion");
	}

	PseudoConvertirDecimal::~PseudoConvertirDecimal(void) {}

	void PseudoConvertirDecimal::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::locale tg;

		if(this->var && (this->locale != "*"))
		{
			tg = std::locale((std::string) data->ObtenerVariable(this->locale));
		}
		else if(this->locale != "*")
		{
			tg = std::locale((std::string) this->locale);
		}
		else
		{
			tg = std::locale();
		}

		PDCadena& vl = data->ObtenerVariable(this->vard);
		PDDecimal dc = caf(vl);

		std::locale nm {};

		std::locale::global(tg);

		vl = dac(dc);

		std::locale::global(nm);
	}

	void PseudoConvertirDecimal::LeerParametros(pseudod::Tokenizador& in)
	{
		pseudod::Token tok1, tok2;

		if(!(in >> tok1 >> tok2))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var': EOF inesperado"
			);
		}

		if(
			(!Tokens::EsIdentificador(tok1)) ||
			(!Tokens::EsIdentificador(tok2))
		)
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale': Se esperaban identificadores"
			);
		}

		this->locale = Tokens::ObtenerValor(tok1);
		this->vard = Tokens::ObtenerValor(tok2);
		this->var = true;
	}

	PseudoConvertirEntero::PseudoConvertirEntero(PDCadena nloc, PDCadena var)
		: locale(nloc), var(false), vard(var)
	{
		this->FijarClave("ConvertirEnteroALocal", "Localizacion");
	}

	PseudoConvertirEntero::~PseudoConvertirEntero(void) {}

	void PseudoConvertirEntero::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::locale tg;

		if(this->var && (this->locale != "*"))
		{
			tg = std::locale((std::string) data->ObtenerVariable(this->locale));
		}
		else if(this->locale != "*")
		{
			tg = std::locale((std::string) this->locale);
		}
		else
		{
			tg = std::locale();
		}

		PDCadena& vl = data->ObtenerVariable(this->vard);
		PDEntero dc = cae(vl);

		std::locale nm {};

		std::locale::global(tg);

		vl = eas(dc);

		std::locale::global(nm);
	}

	void PseudoConvertirEntero::LeerParametros(pseudod::Tokenizador& in)
	{
		pseudod::Token tok1, tok2;

		if(!(in >> tok1 >> tok2))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var': EOF inesperado"
			);
		}

		if(
			(!Tokens::EsIdentificador(tok1)) ||
			(!Tokens::EsIdentificador(tok2))
		)
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale': Se esperaban identificadores"
			);
		}

		this->locale = Tokens::ObtenerValor(tok1);
		this->vard = Tokens::ObtenerValor(tok2);
		this->var = true;
	}

	PseudoConvertirDecimalDesde::PseudoConvertirDecimalDesde(PDCadena nloc, PDCadena var)
		: locale(nloc), var(false), vard(var)
	{
		this->FijarClave("ConvertirDecimalDesdeLocal", "Localizacion");
	}

	PseudoConvertirDecimalDesde::~PseudoConvertirDecimalDesde(void) {}

	void PseudoConvertirDecimalDesde::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::locale tg;

		if(this->var && (this->locale != "*"))
		{
			tg = std::locale((std::string) data->ObtenerVariable(this->locale));
		}
		else if(this->locale != "*")
		{
			tg = std::locale((std::string) this->locale);
		}
		else
		{
			tg = std::locale();
		}

		PDCadena& vl = data->ObtenerVariable(this->vard);

		std::locale cp {};

		std::locale::global(tg);

		PDDecimal dc = caf(vl);

		std::locale::global(cp);

		vl = dac(dc);
	}

	void PseudoConvertirDecimalDesde::LeerParametros(pseudod::Tokenizador& in)
	{
		pseudod::Token tok1, tok2;

		if(!(in >> tok1 >> tok2))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var': EOF inesperado"
			);
		}

		if(
			(!Tokens::EsIdentificador(tok1)) ||
			(!Tokens::EsIdentificador(tok2))
		)
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var': Se esperaban identificadores"
			);
		}

		this->locale = Tokens::ObtenerValor(tok1);
		this->vard = Tokens::ObtenerValor(tok2);
		this->var = true;
	}

	PseudoConvertirEnteroDesde::PseudoConvertirEnteroDesde(PDCadena nloc, PDCadena var)
		: locale(nloc), var(false), vard(var)
	{
		this->FijarClave("ConvertirEnteroDesdeLocal", "Localizacion");
	}

	PseudoConvertirEnteroDesde::~PseudoConvertirEnteroDesde(void) {}

	void PseudoConvertirEnteroDesde::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::locale tg;

		if(this->var && (this->locale != "*"))
		{
			tg = std::locale((std::string) data->ObtenerVariable(this->locale));
		}
		else if(this->locale != "*")
		{
			tg = std::locale((std::string) this->locale);
		}
		else
		{
			tg = std::locale();
		}

		PDCadena& vl = data->ObtenerVariable(this->vard);

		std::locale cp {};

		std::locale::global(tg);

		PDDecimal dc = cae(vl);

		std::locale::global(cp);

		vl = eas(dc);
	}

	void PseudoConvertirEnteroDesde::LeerParametros(pseudod::Tokenizador& in)
	{
		pseudod::Token tok1, tok2;

		if(!(in >> tok1 >> tok2))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var': EOF inesperado"
			);
		}

		if(
			(!Tokens::EsIdentificador(tok1)) ||
			(!Tokens::EsIdentificador(tok2))
		)
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var': Se esperaban identificadores"
			);
		}

		this->locale = Tokens::ObtenerValor(tok1);
		this->vard = Tokens::ObtenerValor(tok2);
		this->var = true;
	}

	PseudoEsCaracter::PseudoEsCaracter(PDCadena nloc, PDCadena var, PDCadena tp)
		: locale(nloc), var(false), vard(var), varh(tp)
	{
		this->FijarClave("TipoCaracter", "Localizacion");
	}

	PseudoEsCaracter::~PseudoEsCaracter(void) {}

	void PseudoEsCaracter::InscribirInstancia(PDvar::PDDatos* data)
	{
		std::locale tg;

		if(this->var && (this->locale != "*"))
		{
			tg = std::locale((std::string) data->ObtenerVariable(this->locale));
		}
		else if(this->locale != "*")
		{
			tg = std::locale((std::string) this->locale);
		}
		else
		{
			tg = std::locale();
		}

		std::locale saved {};

		std::locale::global(tg);

		PDCadena& vl = data->ObtenerVariable(this->vard);
		const PDCadena& tp = data->ObtenerVariable(this->varh);

		auto mtow = [&, this](const std::string& str) -> std::wstring
		{
			const char* src = str.data();
			std::mbstate_t st {};
			std::size_t len = 1 + std::mbsrtowcs(nullptr, &src, 0, &st);
			std::wstring dest;

			if(len == 0)
			{
				throw std::runtime_error("Secuencia multibyte invalida");
			}

			dest.reserve(len);
			dest.insert(0, len, L'\0');

			std::mbsrtowcs(&dest.at(0), &src, len, &st);

			return dest;
		};

		auto wtom = [&, this](const std::wstring& str) -> std::string
		{
			const wchar_t* src = str.data();
			std::mbstate_t st {};
			std::size_t len = 1 + std::wcsrtombs(nullptr, &src, 0, &st);
			std::string dest;

			if(len == 0)
			{
				throw std::runtime_error("Secuencia widebyte invalida");
			}

			dest.reserve(len);
			dest.insert(0, len, '\0');

			std::wcsrtombs(&dest.at(0), &src, len, &st);

			return dest;
		};

		std::wstring wvl = L"";

		try
		{
			wvl = mtow(vl);
		}
		catch(std::runtime_error err)
		{
			std::locale::global(saved);

			throw PDvar::ErrorDeSemantica(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var tp': Secuencia multibyte invalida"
			);
		}

		auto iscll = [&, this](bool (*fcn)(wchar_t, const std::locale&))
		{
			vl = fcn(wvl[0], tg)? data->VERDADERO : data->FALSO;
		};

		auto tocll = [&, this](wchar_t (*fcn)(wchar_t, const std::locale&))
		{
			wvl = fcn(wvl[0], tg);

			try
			{
				vl = wtom(wvl);
			}
			catch(std::runtime_error err)
			{
				std::locale::global(saved);

				throw PDvar::ErrorDeSemantica(
					"Error en "
					+ this->ObtenerClave()
					+ ": '"
					+ this->ObtenerClave()
					+ " locale var tp': Secuencia widebyte invalida"
				);
			}
		};

		if(tp == "ES")
		{
			iscll(&std::isspace<wchar_t>);
		}
		else if(tp == "BL")
		{
			iscll(&std::isblank<wchar_t>);
		}
		else if(tp == "CO")
		{
			iscll(&std::iscntrl<wchar_t>);
		}
		else if(tp == "MA")
		{
			iscll(&std::isupper<wchar_t>);
		}
		else if(tp == "MI")
		{
			iscll(&std::islower<wchar_t>);
		}
		else if(tp == "AL")
		{
			iscll(&std::isalpha<wchar_t>);
		}
		else if(tp == "DI")
		{
			iscll(&std::isdigit<wchar_t>);
		}
		else if(tp == "PU")
		{
			iscll(&std::ispunct<wchar_t>);
		}
		else if(tp == "HE")
		{
			iscll(&std::isxdigit<wchar_t>);
		}
		else if(tp == "AN")
		{
			iscll(&std::isalnum<wchar_t>);
		}
		else if(tp == "VI")
		{
			iscll(&std::isprint<wchar_t>);
		}
		else if(tp == "GR")
		{
			iscll(&std::isgraph<wchar_t>);
		}
		else if(tp == "TA")
		{
			tocll(&std::toupper<wchar_t>);
		}
		else if(tp == "TI")
		{
			tocll(&std::tolower<wchar_t>);
		}

		std::locale::global(saved);
	}

	void PseudoEsCaracter::LeerParametros(pseudod::Tokenizador& in)
	{
		pseudod::Token tok1, tok2, tok3;

		if(!(in >> tok1 >> tok2 >> tok3))
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var tp': EOF inesperado"
			);
		}

		if(
			(!Tokens::EsIdentificador(tok1)) ||
			(!Tokens::EsIdentificador(tok2)) ||
			(!Tokens::EsIdentificador(tok3))
		)
		{
			throw PDvar::ErrorDeSintaxis(
				"Error en "
				+ this->ObtenerClave()
				+ ": '"
				+ this->ObtenerClave()
				+ " locale var tp': Se esperaban identificadores"
			);
		}

		this->locale = Tokens::ObtenerValor(tok1);
		this->vard = Tokens::ObtenerValor(tok2);
		this->varh = Tokens::ObtenerValor(tok3);
		this->var = true;
	}
}
