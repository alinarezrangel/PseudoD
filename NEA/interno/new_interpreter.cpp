/* Nuevo intérprete.
 *
 * Copyright 2016 Alejandro Linarez Rangel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <utility>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <functional>
#include <map>

#include "memory_types.hh"

#include "new_interpreter.hh"

namespace pseudod
{
	namespace TokenUtils = Utilidades::Tokens;

	namespace
	{
		struct DevolverProcedimiento
		{
			ValorPtr valor;
		};

		struct ErrorDeSintaxisConReintento
		{
			PDvar::ErrorDeSintaxis err;
		};

		template<typename F>
		inline auto ConReintento(F f) -> decltype(f())
		{
			try
			{
				return f();
			}
			catch(PDvar::ErrorDeSintaxis err)
			{
				throw ErrorDeSintaxisConReintento {err};
			}
		}
	}

	Interprete::Interprete(
		ConfInterprete conf,
		AmbitoPtr ambito,
		ManejadorDeModulosPtr manejador
	) : ambito(ambito), conf(conf), manejadorDeModulos(manejador)
	{}

	const ConfInterprete& Interprete::ObtenerConfiguracion(void) const
	{
		return this->conf;
	}

	ConfInterprete& Interprete::ObtenerConfiguracion(void)
	{
		return this->conf;
	}

	AmbitoPtr Interprete::ObtenerAmbito(void) const
	{
		return this->ambito;
	}

	ManejadorDeModulosPtr Interprete::ObtenerManejadorDeModulos(void) const
	{
		return this->manejadorDeModulos;
	}

	Interprete Interprete::CrearSubinterprete(void)
	{
		return Interprete(
			this->conf,
			std::make_shared<Ambito>(this->ambito),
			this->manejadorDeModulos
		);
	}

	bool Interprete::EsVerdadero(ValorPtr val)
	{
		if(this->conf.ConvertirTextoABoole)
		{
			if(ValorEs<Boole>(val))
			{
				return static_cast<bool>(*ValorComo<Boole>(val));
			}
			else if(ValorEs<Texto>(val))
			{
				return static_cast<std::string>(*ValorComo<Texto>(val)) == "VERDADERO";
			}
			else
			{
				return false;
			}
		}
		else if(ValorEs<Boole>(val))
		{
			return static_cast<bool>(*ValorComo<Boole>(val));
		}
		else
		{
			throw PDvar::ErrorDeSemantica("Se esperaba un valor booleano.");
		}
	}

#define TRY_STMT(backtracker, nombre) \
	do { \
		auto& nbacktracker = (backtracker); \
		auto iter = nbacktracker.ObtenerIteradorActual(); \
		try \
		{ \
			nombre(nbacktracker); \
			return; \
		} \
		catch(ErrorDeSintaxisConReintento n) \
		{ \
			nbacktracker.IrAIterador(iter); \
		} \
	} while(0)
#define TRY_EXPR(backtracker, nombre) \
	do { \
		auto& nbacktracker = (backtracker); \
		auto iter = nbacktracker.ObtenerIteradorActual(); \
		try \
		{ \
			return nombre(nbacktracker); \
		} \
		catch(ErrorDeSintaxisConReintento n) \
		{ \
			nbacktracker.IrAIterador(iter); \
		} \
	} while(0)

	void Interprete::EjecutarSiguiente(Backtracker& backtracker)
	{
		TRY_STMT(backtracker, this->EjecutarAdquirir);
		TRY_STMT(backtracker, this->EjecutarInstancia);
		TRY_STMT(backtracker, this->EjecutarLiberar);
		TRY_STMT(backtracker, this->EjecutarFijar);
		TRY_STMT(backtracker, this->EjecutarEscribir);
		TRY_STMT(backtracker, this->EjecutarNl);
		TRY_STMT(backtracker, this->EjecutarLeer);
		TRY_STMT(backtracker, this->EjecutarSi);
		TRY_STMT(backtracker, this->EjecutarMientras);
		TRY_STMT(backtracker, this->EjecutarProcedimiento);
		TRY_STMT(backtracker, this->EjecutarDevolver);
		TRY_STMT(backtracker, this->EjecutarUtilizar);
		TRY_STMT(backtracker, this->EjecutarClase);
		TRY_STMT(backtracker, this->EjecutarNecesitas);
		TRY_STMT(backtracker, this->EjecutarAtributo);
		TRY_STMT(backtracker, this->EvaluarSiguiente);

		this->LanzaErrorDeSintaxis(
			this->SiguienteToken(backtracker),
			"Se esperaba una instrucción."
		);
	}

	ValorPtr Interprete::EvaluarSiguiente(Backtracker& backtracker)
	{
		TRY_EXPR(backtracker, this->EvaluarParentesis);
		TRY_EXPR(backtracker, this->EvaluarLiteral);
		TRY_EXPR(backtracker, this->EvaluarSon);
		TRY_EXPR(backtracker, this->EvaluarNo);
		TRY_EXPR(backtracker, this->EvaluarLlamar);
		TRY_EXPR(backtracker, this->EvaluarProcedimiento);
		TRY_EXPR(backtracker, this->EvaluarVariable);
		TRY_EXPR(backtracker, this->EvaluarReferenciaVariable);
		TRY_EXPR(backtracker, this->EvaluarAutoejecucionForzada);
		TRY_EXPR(backtracker, this->EvaluarClonar);

		this->LanzaErrorDeSintaxis(
			this->SiguienteToken(backtracker),
			"Se esperaba una expresión."
		);
	}

	void Interprete::Ejecutar(Backtracker& backtracker)
	{
		while(!backtracker.FinDelPrograma())
		{
			this->EjecutarSiguiente(backtracker);
		}
	}

	void Interprete::Ejecutar(
		const std::vector<Token>& tokens,
		Token::DatosFuente lugar
	)
	{
		Backtracker backtracker(tokens, NuevoTokenizador(lugar));
		this->Ejecutar(backtracker);
	}

	void Interprete::Ejecutar(
		Backtracker::Iterador inicio,
		Backtracker::Iterador final,
		Token::DatosFuente lugar
	)
	{
		std::vector<Token> tokens(inicio, final);
		this->Ejecutar(tokens, lugar);
	}

	ValorPtr Interprete::Evaluar(Backtracker& backtracker)
	{
		return this->EvaluarSiguiente(backtracker);
	}

	ValorPtr Interprete::Evaluar(
		const std::vector<Token>& tokens,
		Token::DatosFuente lugar
	)
	{
		Backtracker backtracker(tokens, NuevoTokenizador(lugar));
		return this->Evaluar(backtracker);
	}

	ValorPtr Interprete::Evaluar(
		Backtracker::Iterador inicio,
		Backtracker::Iterador final,
		Token::DatosFuente lugar
	)
	{
		std::vector<Token> tokens(inicio, final);
		return this->Evaluar(tokens, lugar);
	}

#undef TRY_STMT
#undef TRY_EXPR

	void Interprete::LanzaErrorDeSintaxis(
		Token::DatosFuente lugar,
		const std::string& mensaje
	)
	{
		throw PDvar::ErrorDeSintaxis(
			"En [" + lugar.nombreDelArchivo +
			"]: linea " + std::to_string(lugar.linea) +
			" columna " + std::to_string(lugar.columna) +
			": " + mensaje
		);
	}

	void Interprete::LanzaErrorDeSintaxis(const Token& tok, const std::string& mensaje)
	{
		this->LanzaErrorDeSintaxis(tok.ObtenerDatosFuente(), mensaje);
	}

	void Interprete::FallaSiFinDelFlujo(Backtracker& tok)
	{
		if(tok.FinDelPrograma())
		{
			throw PDvar::ErrorDeSintaxis("Fin del archivo inesperado");
		}
	}

	Token Interprete::LeerToken(Backtracker& tok)
	{
		Token tk = tok.LeerToken();
		if(tk.ObtenerTipo() == Token::SinTipo && tok.FinDelPrograma())
		{
			this->LanzaErrorDeSintaxis(tk, "Fin del archivo inesperado");
		}
		return tk;
	}

	Token Interprete::EsperarIgual(Backtracker& tok, NMemonico::Palabra pal)
	{
		Token t = this->LeerToken(tok);
		if((!TokenUtils::EsNMemonico(t)) || (t.ObtenerNMemonico() != pal))
		{
			this->LanzaErrorDeSintaxis(t, "Se esperaba una palabra clave.");
		}
		return t;
	}

	Token Interprete::EsperarIgual(Backtracker& tok, Token::ValorLiteral val)
	{
		Token t = this->LeerToken(tok);
		if((!TokenUtils::EsLiteral(t)) || (t.ObtenerValorLiteral() != val))
		{
			this->LanzaErrorDeSintaxis(t, "Se esperaba una literal o variable.");
		}
		return t;
	}

	Token Interprete::EsperarIgual(
		Backtracker& tok,
		Token::ValorLiteral::TipoValor tv
	)
	{
		Token t = this->LeerToken(tok);
		if((!TokenUtils::EsLiteral(t)) || (t.ObtenerValorLiteral().tipo != tv))
		{
			this->LanzaErrorDeSintaxis(t, "Se esperaba una literal o variable.");
		}
		return t;
	}

	Token Interprete::LeerNMemonico(Backtracker& tok)
	{
		Token t = this->LeerToken(tok);
		if(!TokenUtils::EsNMemonico(t))
		{
			this->LanzaErrorDeSintaxis(t, "Se esperaba una palabra clave.");
		}
		return t;
	}

	Token Interprete::SiguienteToken(Backtracker& tok)
	{
		auto iter = tok.ObtenerIteradorActual();
		Token t = tok.LeerToken();
		tok.IrAIterador(iter);
		return t;
	}

	bool Interprete::SiguienteTokenEs(Backtracker& tok, NMemonico::Palabra pal)
	{
		auto t = this->SiguienteToken(tok);
		return TokenUtils::EsNMemonico(t) && t.ObtenerNMemonico() == pal;
	}

	bool Interprete::SiguienteTokenEs(
		Backtracker& tok,
		Token::ValorLiteral val
	)
	{
		auto t = this->SiguienteToken(tok);
		return TokenUtils::EsLiteral(t) && t.ObtenerValorLiteral() == val;
	}

	bool Interprete::SiguienteTokenEs(
		Backtracker& tok,
		Token::ValorLiteral::TipoValor tv
	)
	{
		auto t = this->SiguienteToken(tok);
		return TokenUtils::EsLiteral(t)
			&& t.ObtenerValorLiteral().tipo == tv;
	}

	bool Interprete::LeerTokenProcedimiento(Backtracker& tok)
	{
		if(!this->SiguienteTokenEs(tok, NMemonico::PD_FUNCION))
		{
			this->LanzaErrorDeSintaxis(
				this->SiguienteToken(tok),
				"Se esperaba la palabra clave funcion, procedimiento o metodo"
			);
		}
		auto tk = this->LeerToken(tok);
		return tk.ObtenerNMemonico() == NMemonico::PD_CLASE_METODO;
	}

	ValorPtr Interprete::LeerYEnviarMensajes(Backtracker& tok, ValorPtr obj)
	{
		while(this->SiguienteTokenEs(tok, NMemonico::PD_ENVIAR_MENSAJE))
		{
			this->LeerToken(tok);
			Token msgtk = this->EsperarIgual(tok, Token::ValorLiteral::Identificador);
			std::string mensaje = TokenUtils::ObtenerValor(msgtk);
			std::vector<ValorPtr> argumentos;
			if(this->SiguienteTokenEs(tok, NMemonico::PD_OPERADOR_LLAMAR))
			{
				argumentos = this->LeerArgumentosProcedimiento(tok);
				if(this->SiguienteTokenEs(tok, NMemonico::PD_ENVIAR_MENSAJE))
				{
					this->LanzaErrorDeSintaxis(
						this->SiguienteToken(tok),
						"No se puede enviar otro mensaje luego de llamar a un metodo con argumentos."
					);
				}
			}

			obj = obj->RecibirMensaje(mensaje, argumentos);
		}

		while(this->SiguienteTokenEs(tok, NMemonico::PD_OPERADOR_GENERAL))
		{
			auto tk = this->LeerToken(tok);
			std::string op = tk.ObtenerNMemonico().original;

			ValorPtr rhs = this->EvaluarSiguiente(tok);

			obj = obj->RecibirMensaje(
				"operador_" + op,
				std::vector<ValorPtr> { rhs }
			);
		}

		return obj;
	}

	void Interprete::EjecutarAdquirir(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_ADQUIRIR);
		});
		std::vector<std::string> nombres;
		std::string varname = TokenUtils::ObtenerValor(this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		));
		nombres.push_back(varname);

		while(this->SiguienteTokenEs(tok, NMemonico::PD_SEPARADOR_DE_ARGUMENTOS))
		{
			this->LeerToken(tok);

			std::string varname = TokenUtils::ObtenerValor(this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			));
			nombres.push_back(varname);
		}

		for(std::string nombre : nombres)
		{
			this->ambito->CrearVariable(nombre, CrearNulo());
		}
	}

	void Interprete::EjecutarInstancia(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_INSTANCIA);
		});
		ValorPtr type = this->EvaluarSiguiente(tok);
		Token varname = this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		);
		this->ambito->CrearVariable(
			TokenUtils::ObtenerValor(varname),
			type->RecibirMensaje("crear", std::vector<ValorPtr> {})
		);
	}

	void Interprete::EjecutarLiberar(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_LIBERAR);
		});
		Token varname = this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		);
		this->ambito->EliminarVariable(TokenUtils::ObtenerValor(varname));
	}

	void Interprete::EjecutarFijar(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_FIJAR);
		});
		Token varnametk = this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		);
		auto varname = TokenUtils::ObtenerValor(varnametk);
		ValorPtr obj = nullptr;
		std::string atr;

		if(this->SiguienteTokenEs(tok, NMemonico::PD_ENVIAR_MENSAJE))
		{
			this->LeerToken(tok);
			obj = this->ambito->ObtenerVariable(varname);
			atr = TokenUtils::ObtenerValor(
				this->EsperarIgual(
					tok,
					Token::ValorLiteral::Identificador
				)
			);
			while(this->SiguienteTokenEs(tok, NMemonico::PD_ENVIAR_MENSAJE))
			{
				this->LeerToken(tok);
				obj = obj->RecibirMensaje(atr, std::vector<ValorPtr> {});
				atr = TokenUtils::ObtenerValor(
					this->EsperarIgual(
						tok,
						Token::ValorLiteral::Identificador
					)
				);
			}
		}

		this->EsperarIgual(tok, NMemonico::PD_OPERADOR_A);
		ValorPtr val = this->EvaluarSiguiente(tok);

		if(obj != nullptr)
		{
			obj->RecibirMensaje(
				"fijar_" + atr,
				std::vector<ValorPtr> {val}
			);
		}
		else
		{
			this->ambito->FijarVariable(varname, val);
		}
	}

	void Interprete::EjecutarEscribir(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_ESCRIBIR);
		});
		ValorPtr val = this->EvaluarSiguiente(tok);

		auto texto = std::dynamic_pointer_cast<Texto>(val);
		if(texto)
		{
			std::cout << texto->ObtenerTexto();
		}
		else
		{
			throw PDvar::ErrorDeSemantica("Se esperaba texto, se obtuvo otro valor.");
		}
	}

	void Interprete::EjecutarNl(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_NUEVALINEA);
		});
		std::cout << std::endl;
	}

	void Interprete::EjecutarLeer(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_LEER);
		});
		Token varname = this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		);
		std::string word;
		if(!(std::cin >> word))
		{
			throw PDvar::ErrorDeSemantica("EOF inesperado");
		}
		this->ambito->FijarVariable(
			TokenUtils::ObtenerValor(varname),
			CrearValor<Texto>(word)
		);
	}

	void Interprete::EjecutarSi(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_SI);
		});
		ValorPtr cond = this->EvaluarSiguiente(tok);

		auto cur = tok.ObtenerIteradorActual();
		auto fin = tok.IteradorAlFinal();

		auto finsi = cur;
		auto sino = fin;

		for(int depth = 1; finsi != fin; finsi++)
		{
			if(!TokenUtils::EsNMemonico(*finsi))
				continue;

			auto nm = finsi->ObtenerNMemonico();

			if(nm == NMemonico::PD_SI)
			{
				depth ++;
			}

			if(nm == NMemonico::PD_SINO && depth == 1)
			{
				if(sino != fin)
				{
					this->LanzaErrorDeSintaxis(*sino, "Condicional si con más de un sino.");
				}
				sino = finsi;
			}

			if(nm == NMemonico::PD_FIN_SI)
			{
				depth --;
				if(depth == 0)
					break;
			}
		}

		if(finsi == fin)
		{
			this->LanzaErrorDeSintaxis(*cur, "Se esperaba finsi.");
		}

		tok.IrAIterador(finsi + 1);

		auto subinterprete = this->CrearSubinterprete();

		if(this->EsVerdadero(cond))
		{
			subinterprete.Ejecutar(cur, sino == fin? finsi : sino);
		}
		else if(sino != fin)
		{
			subinterprete.Ejecutar(sino + 1, finsi);
		}
	}

	void Interprete::EjecutarMientras(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_MIENTRAS);
		});
		auto condIter = tok.ObtenerIteradorActual();
		ValorPtr condVal = this->EvaluarSiguiente(tok);

		auto cuerpoIter = tok.ObtenerIteradorActual();
		auto fin = tok.IteradorAlFinal();
		auto finBucle = cuerpoIter;

		for(int depth = 1; finBucle != fin; finBucle++)
		{
			if(!TokenUtils::EsNMemonico(*finBucle))
				continue;

			auto nm = finBucle->ObtenerNMemonico();

			if(nm == NMemonico::PD_MIENTRAS)
			{
				depth ++;
			}

			if(nm == NMemonico::PD_FIN_BUCLE)
			{
				depth --;
				if(depth == 0)
					break;
			}
		}

		if(finBucle == fin)
		{
			this->LanzaErrorDeSintaxis(*cuerpoIter, "Se esperaba finmientras");
		}

		while(this->EsVerdadero(condVal))
		{
			auto subinterprete = this->CrearSubinterprete();
			subinterprete.Ejecutar(cuerpoIter, finBucle);
			tok.IrAIterador(condIter);
			condVal = this->EvaluarSiguiente(tok);
		}

		tok.IrAIterador(finBucle + 1);
	}

	void Interprete::EjecutarProcedimiento(Backtracker& tok)
	{
		bool esMetodo = ConReintento([this, &tok]()
		{
			return this->LeerTokenProcedimiento(tok);
		});

		bool estatico = this->SiguienteTokenEs(tok, NMemonico::PD_CLASE_METODO_ESTATICO);
		if(estatico)
			this->LeerToken(tok);

		Token procnametk = this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		);
		PDCadena procname = TokenUtils::ObtenerValor(procnametk);
		ValorPtr clase = nullptr;

		while(this->SiguienteTokenEs(tok, NMemonico::PD_ENVIAR_MENSAJE))
		{
			this->LeerToken(tok);
			if(!clase)
			{
				clase = this->ambito->ObtenerVariable(procname);
			}
			else
			{
				clase = clase->RecibirMensaje(procname, std::vector<ValorPtr> {});
			}
			procnametk = this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			);
			procname = TokenUtils::ObtenerValor(procnametk);
		}

		auto params = this->LeerParametrosProcedimiento(tok, esMetodo);
		auto cuerpo = this->LeerCuerpoDeProcedimiento(tok);

		auto proc = CrearValor<Procedimiento>(*this, params, cuerpo);

		if(!clase)
		{
			this->ambito->CrearVariable(procname, proc);
			this->ambito->MarcarComoAutoEjecutable(procname);
		}
		else if(estatico)
		{
			ValorComo<ObjetoEnPseudoD>(clase)->AgregarMetodo(procname, proc);
		}
		else
		{
			clase->RecibirMensaje(
				"agregarMetodo",
				std::vector<ValorPtr> {
					CrearValor<Texto>(procname),
					proc
				}
			);
		}
	}

	void Interprete::EjecutarDevolver(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_DEVOLVER);
		});
		ValorPtr valor = this->EvaluarSiguiente(tok);
		throw DevolverProcedimiento {valor};
	}

	void Interprete::EjecutarUtilizar(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_UTILIZAR);
		});
		Token modnametk = this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		);
		PDCadena modname = TokenUtils::ObtenerValor(modnametk);
		AmbitoPtr ambito = this->manejadorDeModulos->ImportarModulo(
			modname,
			this->conf,
			this->manejadorDeModulos
		);
		if(this->SiguienteTokenEs(tok, NMemonico::PD_OPERADOR_COMO))
		{
			this->LeerToken(tok);
			Token modvartk = this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			);
			PDCadena modvar = TokenUtils::ObtenerValor(modvartk);
			auto espacio = std::make_shared<EspacioDeNombres>(ambito);

			if(this->ambito->Existe(modvar))
			{
				ValorPtr var = this->ambito->ObtenerVariable(modvar);
				if(!ValorEs<EspacioDeNombres>(var))
				{
					throw PDvar::ErrorDeSemantica(
						"El nombre del modulo de utilizar debe haber sido declarado con otro utilizar"
					);
				}

				auto viejoEspacio = ValorComo<EspacioDeNombres>(var);
				this->ambito->FijarVariable(
					modvar,
					viejoEspacio->UnionDeEspacios(espacio)
				);
			}
			else
			{
				this->ambito->CrearVariable(
					modvar,
					std::static_pointer_cast<Valor>(espacio)
				);
			}
		}
		else
		{
			Ambito::Desambiguador desambiguador =
				[](auto actual, auto nuevo)
				{
					return nuevo;
				};
			this->ambito->Insertar(*ambito, desambiguador);
		}
	}

	void Interprete::EjecutarClase(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_CLASE);
		});
		Token nombreClaseTk = this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		);
		std::string nombreClase = TokenUtils::ObtenerValor(nombreClaseTk);

		ValorPtr clase;

		if(this->SiguienteTokenEs(tok, NMemonico::PD_HEREDAR))
		{
			this->LeerToken(tok);
			ValorPtr claseBase = this->EvaluarSiguiente(tok);

			clase = claseBase->RecibirMensaje("subclase", std::vector<ValorPtr> {});
		}
		else
		{
			if(this->conf.ClaseObjeto == nullptr)
			{
				throw PDvar::ErrorDelNucleo(
					"(Error Interno) No se configuro una clase base global para el interprete de PseudoD"
				);
			}

			clase = this->conf.ClaseObjeto->RecibirMensaje(
				"subclase",
				std::vector<ValorPtr> {}
			);
		}

		while(
			this->SiguienteTokenEs(tok, NMemonico::PD_CLASE_ATRIBUTO) ||
			this->SiguienteTokenEs(tok, NMemonico::PD_CLASE_METODO) ||
			this->SiguienteTokenEs(tok, NMemonico::PD_IMPLEMENTAR)
		)
		{
			if(this->SiguienteTokenEs(tok, NMemonico::PD_CLASE_ATRIBUTO))
			{
				this->LeerToken(tok);

				std::vector<std::string> atributos;

				std::string nombreAtributo = TokenUtils::ObtenerValor(this->EsperarIgual(
					tok,
					Token::ValorLiteral::Identificador
				));

				atributos.push_back(nombreAtributo);

				while(this->SiguienteTokenEs(tok, NMemonico::PD_SEPARADOR_DE_ARGUMENTOS))
				{
					this->LeerToken(tok);
					std::string nombreAtributo = TokenUtils::ObtenerValor(this->EsperarIgual(
						tok,
						Token::ValorLiteral::Identificador
					));

					atributos.push_back(nombreAtributo);
				}

				for(std::string atributo : atributos)
				{
					clase->RecibirMensaje(
						"agregarAtributo",
						std::vector<ValorPtr> {
							CrearValor<Texto>(atributo)
						}
					);
				}
			}
			else if(this->SiguienteTokenEs(tok, NMemonico::PD_CLASE_METODO))
			{
				this->LeerToken(tok);
				if(this->SiguienteTokenEs(tok, NMemonico::PD_CLASE_METODO_ESTATICO))
				{
					this->LeerToken(tok);
				}

				this->EsperarIgual(
					tok,
					Token::ValorLiteral::Identificador
				);

				this->LeerParametrosProcedimiento(tok, false);
			}
			else
			{
				this->LeerToken(tok);

				this->EsperarIgual(
					tok,
					Token::ValorLiteral::Identificador
				);
			}
		}

		clase->RecibirMensaje(
			"fijar_nombre",
			std::vector<ValorPtr> { CrearValor<Texto>(nombreClase) }
		);
		this->EsperarIgual(tok, NMemonico::PD_FIN_CLASE);
		this->ambito->CrearVariable(nombreClase, clase);
	}

	void Interprete::EjecutarNecesitas(Backtracker& tok)
	{
		auto necesitasKw = ConReintento([this, &tok]()
		{
			auto tk = this->SiguienteToken(tok);
			this->EsperarIgual(tok, NMemonico::PD_NECESITAS);
			return tk;
		});
		auto valor = this->EvaluarSiguiente(tok);
		if(!ValorEs<Boole>(valor) || !ValorComo<Boole>(valor)->ObtenerBool())
		{
			Token::DatosFuente lugar = necesitasKw.ObtenerDatosFuente();
			throw PDvar::ErrorDelNucleo(
				"En [" + lugar.nombreDelArchivo +
				"]: linea " + std::to_string(lugar.linea) +
				" columna " + std::to_string(lugar.columna) +
				": Instrucción \"necesitas\" fallida."
			);
		}
	}

	void Interprete::EjecutarAtributo(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_CLASE_ATRIBUTO);
		});

		auto leerNombreAtributo = [this, &tok]()
		{
			std::string nombreClase = TokenUtils::ObtenerValor(this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			));
			this->EsperarIgual(tok, NMemonico::PD_ENVIAR_MENSAJE);
			std::string nombreAtributo = TokenUtils::ObtenerValor(this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			));
			return std::make_pair(
				this->ambito->ObtenerVariable(nombreClase),
				nombreAtributo
			);
		};

		std::vector<std::pair<ValorPtr, std::string>> atributos;

		atributos.push_back(leerNombreAtributo());

		while(this->SiguienteTokenEs(tok, NMemonico::PD_SEPARADOR_DE_ARGUMENTOS))
		{
			this->LeerToken(tok);
			atributos.push_back(leerNombreAtributo());
		}

		for(auto atributo : atributos)
		{
			atributo.first->RecibirMensaje(
				"agregarAtributo",
				std::vector<ValorPtr> {
					CrearValor<Texto>(atributo.second)
				}
			);
		}
	}

	ValorPtr Interprete::EvaluarVariable(Backtracker& tok)
	{
		Token varnametk = ConReintento([this, &tok]()
		{
			return this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			);
		});
		PDCadena varname = TokenUtils::ObtenerValor(varnametk);
		ValorPtr obj = this->ambito->ObtenerVariable(varname);
		if(this->ambito->DebeAutoEjecutarse(varname))
		{
			obj = this->LlamaConParametros(tok, obj);
		}
		else if(this->SiguienteTokenEs(tok, NMemonico::PD_OPERADOR_LLAMAR))
		{
			this->LanzaErrorDeSintaxis(
				varnametk,
				"No se puede llamar a una variable que no es autoejecutable."
			);
		}
		return this->LeerYEnviarMensajes(tok, obj);
	}

	ValorPtr Interprete::EvaluarParentesis(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_PARENTESIS_IZQUIERDO);
		});
		auto val = this->EvaluarSiguiente(tok);
		this->EsperarIgual(tok, NMemonico::PD_PARENTESIS_DERECHO);
		return this->LeerYEnviarMensajes(tok, val);
	}

	ValorPtr Interprete::EvaluarLiteral(Backtracker& tok)
	{
		ValorPtr literal = ConReintento([this, &tok]()
		{
			if(this->SiguienteTokenEs(tok, Token::ValorLiteral::Cadena))
			{
				Token texto = this->EsperarIgual(tok, Token::ValorLiteral::Cadena);
				return CrearValor<Texto>(TokenUtils::ObtenerValor(texto));
			}
			else
			{
				std::string numero =
					TokenUtils::ObtenerValor(this->EsperarIgual(
						tok,
						Token::ValorLiteral::Numero
					));
				if(numero.find(".") != std::string::npos)
				{
					return CrearValor<Numero>(std::stold(numero), Numero::MARCA_TIPO_REAL);
				}
				else
				{
					return CrearValor<Numero>(std::stoll(numero), Numero::MARCA_TIPO_ENTERO);
				}
			}
		});
		return this->LeerYEnviarMensajes(tok, literal);
	}

	ValorPtr Interprete::EvaluarSon(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_OPERADOR_SON);
		});
		Token kwtk = this->LeerNMemonico(tok);
		auto kw = kwtk.ObtenerNMemonico();

		bool igualdad = false;
		if(kw == NMemonico::PD_OPERADOR_IGUALES)
		{
			igualdad = true;
		}
		else if(kw == NMemonico::PD_OPERADOR_DIFERENTES)
		{
			igualdad = false;
		}
		else
		{
			this->LanzaErrorDeSintaxis(kwtk, "Se esperaba iguales/diferentes luego de son/sean.");
		}

		ValorPtr left = this->EvaluarSiguiente(tok);
		this->EsperarIgual(tok, NMemonico::PD_OPERADOR_Y);
		ValorPtr right = this->EvaluarSiguiente(tok);

		auto iguales = left->RecibirMensaje("igualA", { right });

		if(!ValorEs<Boole>(iguales))
		{
			throw PDvar::ErrorDelNucleo("Se esperaba un Boole");
		}

		if(igualdad)
		{
			return iguales;
		}
		else
		{
			return CrearValor<Boole>(
				!ValorComo<Boole>(iguales)->ObtenerBool()
			);
		}
	}

	ValorPtr Interprete::EvaluarNo(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_OPERADOR_NO);
		});
		auto val = this->EvaluarSiguiente(tok);
		if(!ValorEs<Boole>(val))
		{
			throw PDvar::ErrorDelNucleo("Se esperaba un Boole");
		}
		return CrearValor<Boole>(
			!ValorComo<Boole>(val)->ObtenerBool()
		);
	}

	ValorPtr Interprete::EvaluarLlamar(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_LLAMAR);
		});
		PDCadena varname = TokenUtils::ObtenerValor(this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		));
		ValorPtr obj = this->ambito->ObtenerVariable(varname);
		std::string mensaje = "llamar";

		if(this->SiguienteTokenEs(tok, NMemonico::PD_ENVIAR_MENSAJE))
		{
			this->LeerToken(tok);
			mensaje = TokenUtils::ObtenerValor(this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			));

			while(this->SiguienteTokenEs(tok, NMemonico::PD_ENVIAR_MENSAJE))
			{
				this->LeerToken(tok);
				obj = obj->RecibirMensaje(mensaje, std::vector<ValorPtr> {});
				mensaje = TokenUtils::ObtenerValor(this->EsperarIgual(
					tok,
					Token::ValorLiteral::Identificador
				));
			}
		}

		std::vector<ValorPtr> argumentos;

		while(true)
		{
			if(this->SiguienteTokenEs(tok, NMemonico::PD_FIN_ARGUMENTOS))
			{
				this->LeerToken(tok);
				break;
			}
			else
			{
				argumentos.push_back(this->EvaluarSiguiente(tok));
				if(this->SiguienteTokenEs(tok, NMemonico::PD_SEPARADOR_DE_ARGUMENTOS))
				{
					this->LeerToken(tok);
				}
			}
		}

		return obj->RecibirMensaje(mensaje, argumentos);
	}

	ValorPtr Interprete::EvaluarProcedimiento(Backtracker& tok)
	{
		bool esMetodo = ConReintento([this, &tok]()
		{
			return this->LeerTokenProcedimiento(tok);
		});
		auto params = this->LeerParametrosProcedimiento(tok, esMetodo);
		auto cuerpo = this->LeerCuerpoDeProcedimiento(tok);
		auto proc = CrearValor<Procedimiento>(*this, params, cuerpo);
		return this->LeerYEnviarMensajes(tok, proc);
	}

	ValorPtr Interprete::EvaluarReferenciaVariable(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_REFERENCIA_VARIABLE);
		});
		PDCadena varname = TokenUtils::ObtenerValor(this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		));
		auto obj = this->ambito->ObtenerVariable(varname);
		return this->LeerYEnviarMensajes(tok, obj);
	}

	ValorPtr Interprete::EvaluarAutoejecucionForzada(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_AUTOEJECUTA_VARIABLE);
		});
		PDCadena varname = TokenUtils::ObtenerValor(this->EsperarIgual(
			tok,
			Token::ValorLiteral::Identificador
		));
		auto obj = this->ambito->ObtenerVariable(varname);
		obj = this->LlamaConParametros(tok, obj);
		return this->LeerYEnviarMensajes(tok, obj);
	}

	ValorPtr Interprete::EvaluarClonar(Backtracker& tok)
	{
		ConReintento([this, &tok]()
		{
			this->EsperarIgual(tok, NMemonico::PD_CLONAR);
		});
		auto obj = this->EvaluarSiguiente(tok);
		obj = obj->RecibirMensaje("clonar", std::vector<ValorPtr>{});
		this->EsperarIgual(tok, NMemonico::PD_CLONAR_CON);
		std::map<std::string, ValorPtr> cambios;
		while(true)
		{
			if(this->SiguienteTokenEs(tok, NMemonico::PD_FIN_CLONAR))
			{
				this->LeerToken(tok);
				break;
			}

			PDCadena attrname = TokenUtils::ObtenerValor(this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			));
			this->EsperarIgual(tok, NMemonico::PD_OPERADOR_LLAMAR);
			ValorPtr val = this->EvaluarSiguiente(tok);
			cambios[attrname] = val;
		}

		for(auto pares : cambios)
		{
			obj->RecibirMensaje(
				"fijar_" + pares.first,
				std::vector<ValorPtr> { pares.second }
			);
		}

		return obj;
	}

	ValorPtr Interprete::LlamaConParametros(Backtracker& tok, ValorPtr obj)
	{
		std::vector<ValorPtr> argumentos;
		if(this->SiguienteTokenEs(tok, NMemonico::PD_OPERADOR_LLAMAR))
		{
			argumentos = this->LeerArgumentosProcedimiento(tok);
		}
		return obj->RecibirMensaje("llamar", argumentos);
	}

	std::vector<ParametroProcedimiento>
	Interprete::LeerParametrosProcedimiento(Backtracker& tok, bool esMetodo)
	{
		std::vector<ParametroProcedimiento> params;

		if(esMetodo)
		{
			params.push_back(ParametroProcedimiento { "yo", false });
		}

		if(!this->SiguienteTokenEs(tok, NMemonico::PD_CON)
			&& !this->SiguienteTokenEs(tok, NMemonico::PD_OPERADOR_LLAMAR))
		{
			return params;
		}
		this->LeerToken(tok);

		while(true)
		{
			bool variadic = false;

			if(this->SiguienteTokenEs(tok, NMemonico::PD_PUNTO))
			{
				this->LeerToken(tok);
				this->EsperarIgual(tok, NMemonico::PD_PUNTO);
				this->EsperarIgual(tok, NMemonico::PD_PUNTO);
				variadic = true;
			}

			ParametroProcedimiento param;
			param.nombre = TokenUtils::ObtenerValor(this->EsperarIgual(
				tok,
				Token::ValorLiteral::Identificador
			));
			param.variadic = variadic;
			params.push_back(param);

			if(variadic)
			{
				break;
			}

			if(!this->SiguienteTokenEs(tok, NMemonico::PD_SEPARADOR_DE_ARGUMENTOS))
			{
				break;
			}
			this->EsperarIgual(tok, NMemonico::PD_SEPARADOR_DE_ARGUMENTOS);
		}

		return params;
	}

	std::vector<ValorPtr>
	Interprete::LeerArgumentosProcedimiento(Backtracker& tok)
	{
		this->EsperarIgual(tok, NMemonico::PD_OPERADOR_LLAMAR);
		std::vector<ValorPtr> argumentos;
		while(true)
		{
			argumentos.push_back(this->EvaluarSiguiente(tok));
			if(this->SiguienteTokenEs(tok, NMemonico::PD_SEPARADOR_DE_ARGUMENTOS))
			{
				this->LeerToken(tok);
			}
			else
			{
				break;
			}
		}
		return argumentos;
	}

	Tokenizador::ListaTokens Interprete::LeerCuerpoDeProcedimiento(
		Backtracker& tok
	)
	{
		auto cur = tok.ObtenerIteradorActual();
		auto fin = tok.IteradorAlFinal();
		auto finproc = cur;

		for(int depth = 1; finproc != fin; finproc++)
		{
			if(!TokenUtils::EsNMemonico(*finproc))
				continue;

			auto nm = finproc->ObtenerNMemonico();

			if(nm == NMemonico::PD_FUNCION)
			{
				depth ++;
			}

			if(nm == NMemonico::PD_FIN_FUNCION)
			{
				depth --;
				if(depth == 0)
					break;
			}
		}

		if(finproc == fin)
		{
			this->LanzaErrorDeSintaxis(*cur, "Se esperaba finprocedimiento");
		}

		tok.IrAIterador(finproc + 1);

		return Tokenizador::ListaTokens(cur, finproc);
	}

	Procedimiento::Procedimiento(
		Interprete interp,
		const std::vector<ParametroProcedimiento>& params,
		const std::pair<ParametroProcedimiento, bool> paramVariadic,
		const std::vector<Token>& cuerpo
	) :
		Valor(),
		parametrosFijos(params),
		parametroVariadic(paramVariadic.first),
		tieneParametroVariadic(paramVariadic.second),
		cuerpo(cuerpo),
		interprete(interp)
	{}

	Procedimiento::Procedimiento(
		Interprete interp,
		const std::vector<ParametroProcedimiento>& paramsYVariadic,
		const std::vector<Token>& cuerpo
	) : Valor(), parametrosFijos(), parametroVariadic(), cuerpo(cuerpo), interprete(interp)
	{
		if(!paramsYVariadic.empty())
		{
			ParametroProcedimiento ultimo = paramsYVariadic.back();
			if(ultimo.variadic)
			{
				this->tieneParametroVariadic = true;
				this->parametroVariadic = ultimo;
				for(auto i = 0; i < (paramsYVariadic.size() - 1); i++)
				{
					this->parametrosFijos.push_back(paramsYVariadic[i]);
				}
			}
			else
			{
				this->tieneParametroVariadic = false;
				this->parametrosFijos = paramsYVariadic;
			}
		}
		else
		{
			this->tieneParametroVariadic = false;
			this->parametrosFijos = paramsYVariadic;
		}
	}

	Procedimiento::~Procedimiento(void)
	{}

	ValorPtr Procedimiento::RecibirMensaje(
		std::string mensaje,
		const std::vector<ValorPtr>& argumentos
	)
	{
		if(mensaje == "llamar")
		{
			return this->Ejecutar(argumentos);
		}
		else if(mensaje == "comoTexto")
		{
			EsperarNingunArgumento(argumentos);
			return CrearValor<Texto>("#<procedimiento>");
		}
		else if(mensaje == "clonar")
		{
			EsperarNingunArgumento(argumentos);
			return this->shared_from_this();
		}
		else if(mensaje == "igualA" || mensaje == "operador_=")
		{
			EsperaNumArgumentos(argumentos, 1);
			return CrearValor<Boole>(this->shared_from_this() == argumentos[0]);
		}

		throw PDvar::ErrorDelNucleo(
			"No se encontro el mensaje " + mensaje + " en la instancia de Procedimiento"
		);
	}

	const std::vector<ParametroProcedimiento>
	Procedimiento::ObtenerParametrosFijos(void) const
	{
		return this->parametrosFijos;
	}

	const std::pair<ParametroProcedimiento, bool>
	Procedimiento::ObtenerParametroVariadic(void) const
	{
		return std::make_pair(this->parametroVariadic, this->tieneParametroVariadic);
	}

	const std::vector<Token>& Procedimiento::ObtenerTokens(void) const
	{
		return this->cuerpo;
	}

	Interprete Procedimiento::ObtenerInterprete(void) const
	{
		return this->interprete;
	}

	ValorPtr Procedimiento::Ejecutar(const std::vector<ValorPtr>& args)
	{
		bool esValidoParaVariadic =
			this->tieneParametroVariadic && args.size() >= this->parametrosFijos.size();
		bool esValidoSinVariadic =
			!this->tieneParametroVariadic && args.size() == this->parametrosFijos.size();
		if(!esValidoParaVariadic && !esValidoSinVariadic)
		{
			throw PDvar::ErrorDelNucleo(
				"Se esperaban " + std::to_string(this->parametrosFijos.size()) +
				(this->tieneParametroVariadic? " o mas" : "") +
				" parametros pero se recibieron " +
				std::to_string(args.size())
			);
		}

		auto interp = this->interprete.CrearSubinterprete();
		auto ambito = interp.ObtenerAmbito();

		for(size_t i = 0; i < this->parametrosFijos.size(); i++)
		{
			auto param = this->parametrosFijos[i];
			auto arg = args[i];

			ambito->CrearVariable(param.nombre, arg);
		}

		if(this->tieneParametroVariadic)
		{
			std::vector<ValorPtr> extraArgs;
			for(size_t i = this->parametrosFijos.size(); i < args.size(); i++)
			{
				extraArgs.push_back(args[i]);
			}
			ambito->CrearVariable(
				this->parametroVariadic.nombre,
				CrearValor<Arreglo>(extraArgs)
			);
		}

		try
		{
			interp.Ejecutar(this->cuerpo);
		}
		catch(DevolverProcedimiento dev)
		{
			return dev.valor;
		}

		return CrearNulo();
	}

	ManejadorDeModulos::ManejadorDeModulos(ConfManejadorDeModulos conf)
		: conf(conf), modulosImportados()
	{}

	const ConfManejadorDeModulos& ManejadorDeModulos::ObtenerConfiguracion(void) const
	{
		return this->conf;
	}

	ConfManejadorDeModulos& ManejadorDeModulos::ObtenerConfiguracion(void)
	{
		return this->conf;
	}

	AmbitoPtr ManejadorDeModulos::ImportarModulo(
		std::string nombre,
		ConfInterprete conf,
		ManejadorDeModulosPtr manejadorDeModulos
	)
	{
		if(this->modulosImportados.count(nombre) > 0)
		{
			return this->modulosImportados[nombre].ambito;
		}
		AmbitoPtr ambito;
		if(conf.AmbitoBase)
			ambito = std::make_shared<Ambito>(conf.AmbitoBase);
		else
			ambito = std::make_shared<Ambito>();
		Interprete interp {conf, ambito, manejadorDeModulos};
		const std::vector<std::string> extensiones = {
			".pseudo",
			".pd",
			".psd"
		};
		std::ifstream in;
		std::string nombreDelArchivo;
		for(const auto& ext : extensiones)
		{
			nombreDelArchivo = nombre + ext;
			in = std::ifstream(nombreDelArchivo);
			if(in)
			{
				break;
			}
		}
		Backtracker tok(NuevoTokenizador(Token::DatosFuente(1, 1, nombreDelArchivo)));
		if(!in)
		{
			throw PDvar::ErrorDeSintaxis(
				"No se pudo abrir el archivo del modulo " + nombre
			);
		}
		tok.TokenizarFlujo(in);
		interp.Ejecutar(tok);
		this->modulosImportados[nombre] =
			ManejadorDeModulos::InfoModulo {interp.ObtenerAmbito()};
		return interp.ObtenerAmbito();
	}
}
