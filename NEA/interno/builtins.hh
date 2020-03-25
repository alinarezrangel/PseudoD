/* Variables, datos y procedimientos generales en PseudoD.
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

#ifndef HDR_PSEUDOD_BUILTINS_HH
#define HDR_PSEUDOD_BUILTINS_HH

#include "data.hh"
#include "memory.hh"

namespace pseudod
{
	ValorPtr CrearClaseObjeto(void);

	void RegistrarBuiltins(AmbitoPtr, ValorPtr);
}

#endif /* end of include guard: HDR_PSEUDOD_BUILTINS_HH */
