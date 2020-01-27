/* Copyright 2016 Alejandro Linarez Rangel
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

#include "memory.hh"

namespace pseudod
{
	Valor::Valor(void)
	{}

	Valor::~Valor(void)
	{}

	Ambito::Ambito(void) : variables(), padre()
	{}

	Ambito::Ambito(std::shared_ptr<Ambito> p) : variables(), padre(p)
	{}

	ValorPtr Ambito::operator[](PDCadena nombre) const
	{
		return this->ObtenerVariable(nombre);
	}

	ValorPtr Ambito::ObtenerVariable(PDCadena nombre) const
	{
		return this->ObtenerVariable(nombre, Ambito::ObtenerDatos).valor;
	}

	const Ambito::DatosDeVariable
	Ambito::ObtenerVariable(PDCadena nombre, Ambito::EtiquetaObtenerDatos) const
	{
		if(this->ExisteAqui(nombre))
		{
			return this->variables.at(nombre);
		}
		else if(this->padre)
		{
			return this->padre->ObtenerVariable(nombre, Ambito::ObtenerDatos);
		}
		else
		{
			throw PDvar::ErrorDelNucleo("La variable " + nombre + " no existe.");
		}
	}

	void Ambito::FijarVariable(PDCadena nombre, ValorPtr val)
	{
		if(this->ExisteAqui(nombre))
		{
			this->variables.at(nombre).valor = val;
		}
		else if(this->padre)
		{
			this->padre->FijarVariable(nombre, val);
		}
		else
		{
			throw PDvar::ErrorDelNucleo("La variable " + nombre + " no existe.");
		}
	}

	void Ambito::CrearVariable(PDCadena nombre, ValorPtr valorInicial)
	{
		if(this->ExisteAqui(nombre))
		{
			throw PDvar::ErrorDelNucleo("La variable " + nombre + " ya existe.");
		}
		else
		{
			this->variables[nombre] = Ambito::DatosDeVariable {valorInicial, false};
		}
	}

	AmbitoPtr Ambito::ObtenerAmbitoPadre(void) const
	{
		return this->padre;
	}

	void Ambito::EliminarVariable(PDCadena nombre)
	{
		if(this->ExisteAqui(nombre))
		{
			this->variables.erase(this->variables.find(nombre));
		}
		else if(this->padre)
		{
			this->padre->EliminarVariable(nombre);
		}
		else
		{
			throw PDvar::ErrorDelNucleo("La variable " + nombre + " no existe.");
		}
	}

	bool Ambito::Existe(PDCadena nombre) const
	{
		return this->ExisteAqui(nombre)
			|| (this->padre && this->padre->Existe(nombre));
	}

	bool Ambito::DebeAutoEjecutarse(PDCadena nombre) const
	{
		if(this->ExisteAqui(nombre))
		{
			return this->variables.at(nombre).autoEjecutable;
		}
		else if(this->padre)
		{
			return this->padre->DebeAutoEjecutarse(nombre);
		}
		else
		{
			throw PDvar::ErrorDelNucleo("La variable " + nombre + " no existe.");
		}
	}

	void Ambito::MarcarComoAutoEjecutable(PDCadena nombre)
	{
		if(this->ExisteAqui(nombre))
		{
			this->variables.at(nombre).autoEjecutable = true;
		}
		else if(this->padre)
		{
			this->padre->MarcarComoAutoEjecutable(nombre);
		}
		else
		{
			throw PDvar::ErrorDelNucleo("La variable " + nombre + " no existe.");
		}
	}

	void Ambito::Insertar(Ambito otro, Ambito::Desambiguador desambiguador)
	{
		if(otro.padre && otro.padre != this->padre)
		{
			this->Insertar(*otro.padre, desambiguador);
		}
		for(const auto& otroPar : otro.variables)
		{
			auto nombre = otroPar.first;
			auto valor = otroPar.second;
			if(this->variables.count(nombre) > 0)
			{
				this->variables[nombre] = desambiguador(this->variables[nombre], valor);
			}
			else
			{
				this->variables[nombre] = valor;
			}
		}
	}

	bool Ambito::ExisteAqui(PDCadena nombre) const
	{
		return this->variables.count(nombre) > 0;
	}
}
