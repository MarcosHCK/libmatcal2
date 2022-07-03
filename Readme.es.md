# Descripción del proyecto
## Objetos públicos
- `Matcal`.`Core`
  - Es la interfaz pública por excelencia de la biblioteca. Internamente mantiene una referencia `head` al primer elemento de una lista de `Matcal`.`Object` más un entero `top` que indica la profundidad de la stack. La principal motivación para que la última posición de la stack sea el primer objeto es la velocidad. En el peor de los casos para acceder al elemento `n` en una lista *simplemente enlazada* es O(n), porque hay que recorrer la lista elemento por elemento para alcanzar el `n`; en el caso de una stack casi toda la acción está concentrada en los últimos elementos de ésta (el top de la stack), así que en `Matcal`.`Core` el top y la base de la stack están invertidos.
    > top = 4 \
    > head = *primer objeto*

    > *primer objeto* = *posición 3 en la stack* \
    > | \
    > *segundo objeto* = *posición 2 en la stack* \
    > | \
    > *tercer objeto* = *posición 1 en la stack* \
    > | \
    > *tercer objeto* = *posición 0 en la stack*
  - Nótese que esta biblioteca implementa un sistema de tipos propios, ya que `Matcal`.`Core` implementa una máquina virtual basada en stack. Los tipos que están implementados por defecto en la librería son: valores nulos (`Matcal`.`Nil`), funciones de orden superior (`Matcal`.`Closure`) y valores numéricos de precisión infinita (`Matcal`.`Number`)
  - `Matcal`.`Core`::`Rules` es una propiedad de tipo `Matcal.Rules` y contiene las reglas de análisis de las expresiones matemáticas (vea la entrada correspondiente).
  - `Matcal`.`Core`::`Top` es una propiedad (en los bindings para C#, en C son dos funciones independientes) que manipulan el top de la stack interna de `Matcal`.`Core`. Cambiar el valor del top a un valor más bajo hará que stack destruirá los valor mayores que el nuevo top, en caso contrario agregará valos nulos hasta llenar el agujero
  - `Matcal`.`Core`.`Call` (**int** n_args, **int** n_returns) ejecuta una función a la que le pasará **n_args** argumentos y se asegurarán **n_returns** elementos en la stack después de que se ejecute. La estructura de la stack antes de la llamada debe ser: agregar primero la función, luego **n_args** argumentos. ***Debe*** tener **n_args** argumentos antes de la función o el la llamada fallará (agregue valores nulos de ser necesario)
  - `Matcal`.`Core`.`GetGlobal` (**string** name) agrega a la stack una copia del valor global llamado **name**
  - `Matcal`.`Core`.`Insert` (**int** index) inserta el elemento en el top de la stack (el elemento `Matcal`.`Core`::`Top` - 1) y lo inserta en el índice **index**. Si el índice es mayor que el top agregará valor nulos hasta llenar el agujero.
  - `Matcal`.`Core`.`IsFunction` (**int** index) retorna **true** si el elemento en el índice **index** es de tipo función
  - `Matcal`.`Core`.`IsNil` (**int** index) retorna **true** si el elemento en el índice **index** es nulo
  - `Matcal`.`Core`.`IsNumber` (**int** index) retorna **true** si el elemento en el índice **index** es de tipo número
  - `Matcal`.`Core`.`LoadString` (**string** code) carga una expresión matemática en **code** y la transforma en una función que puede ser ejecutada en la máquina virtual. Si la expresión tiene variables libres serán tratadas como argumentos, y están ordenadas según aparezcan
  - `Matcal`.`Core`.`Pop` (**int** n) destruye los últimos **n** elementos de la stack
  - `Matcal`.`Core`.`PushClosure` (**Matcal**.**CFunction** cclosure) agrega un elemento de tipo función en la stack, que mapea a una función nativa (una de C# o C) con la firma `Matcal`.`CFunction`
  - `Matcal`.`Core`.`PushClosure` (**Matcal**.**CFunction** cclosure, **int** upvalues) similar a la anterior, pero toma *upvalues** elementos de la stack y los agrega como *upvalues* de la función (la variables locales libres de una función de primera clase) que pueden ser utilizados de forma independiente a los argumentos y siempre están disponibles para esta (hasta que la función se destruya)
  - `Matcal`.`Core`.`PushNil` () agrega a la stack un valor nulo
  - `Matcal`.`Core`.`PushNumber` (**double** value) agrega a la stack un valor numérico en la stack, cuyo valor se toma del argumento **value**
  - `Matcal`.`Core`.`PushNumber` (**string** value, **int** base) agrega a la stack un valor numérico en la stack, cuyo valor se toma del argumento **value**. Como el tipo **string** puede almacenar una amplia variedad de representaciones (por ejemplo: enteros ("34823809770"), racionales ("1/4" o "0.25")) y en cualquier base numérica, **base** es la base numérica en la que esta representado el número (10 para la base decimal, 16 para los hexadecimales, etc.)
  - `Matcal`.`Core`.`PushNumber` (**int** value) agrega a la stack un valor numérico en la stack, cuyo valor se toma del argumento **value**
  - `Matcal`.`Core`.`PushUpValue` (**int** index) agrega en la stack el *upvalue* con índice **index** asociado a la función (por supuesto esto solo funciona si se llama desde dentro de una `Matcal`.`CFunction`)
  - `Matcal`.`Core`.`PushValue` (**int** index) agrega en la stack el elemento **index** de la stack (el elemento no se mueve sino que se copia)
  - `Matcal`.`Core`.`RegisterFunction` (**string** name, **uint** n_args) toma la función en el top de la stack y la registra como **name** para ser llamada dentro de una expresión matemática (si registras **jump** luego podrás cargar "**6 * 8 + jump (5)**"). Por ahora las funciones están limitadas a aceptar siempre **n_args** argumentos (de todas formas la practica totalidad de funciones matemáticas se comportan así) pero esto puede cambiar en el futuro
  - `Matcal`.`Core`.`Remove` (**int** index) se comporta como `Matcal`.`Core`.`Pop` pero se puede especificar que índice destruir
  - `Matcal`.`Core`.`SetGlobal` (**string** name) toma el valor en el top de la stack y lo registra globalmente como **name**
  - `Matcal`.`Core`.`ToNumber` (**int** index) retorna una representación del valor numérico del valor un el índice **index** de la stack. Como esta función esta sobrecargada el retorno puede ser de tipo **double** (con lo que se pierde precisión), **int** (también pierde precisión, además si el numero es fraccionario o real se toma la parte entera) o **string** (que almacena la *totalidad* del número, por lo que la cadena resultante puede ser realmente larga en ocasiones)
- `Matcal`.`Expression`
  - Este objeto es una representación de una expresión matemática (ie: **3 + x^2**). Para crearse toma un objeto `Matcal`.`Rules`, que almacena las reglas para analizar dicha expresión. internamente almacena una árbol sintáctico (AST por las siglas de *Abstract Syntax Tree*) que guarda la información pertinente (la propia estructura almacena el orden de ejecución y los nodos almacenan las operaciones, constantes y variables), además de la representación *nativa* (eso es, en código nativo de la maquina en la que se está ejecutando). Esto último se logra compilándose usando compilador JIT (*Just In Time* o *Justo a Tiempo*) implementado usando [DynASM](http://luajit.org/dynasm.html) (es un generador de JITs, forma parte del proyecto **LuaJIT**)
  - `Matcal`.`Expression`::`Ast` es la representación en AST de la expresión matemática
  - `Matcal`.`Expression`.`Compile` () compila la representación AST de la expresión a código nativo de la plataforma en la que se ejecuta, usando la propia API de esta librería
  - `Matcal`.`Expression`.`Push` (`Matcal`.`Core` core) agrega a la stack de **core** la función *nativa* almacenada en la expresión (como esta representación *nativa* no esta disponible hasta que se llama al método `Compile`, no intente llamar esta función antes)
- `Matcal`.`Rules`
  - Almacena las reglas sintácticas para analizar una expresión matemática. Por ejemplo, almacena los operadores y funciones reconocibles, y como separar los variables y las constantes (números) del resto. Por supuesto, es el encargado (internamente, esta API no está disponible públicamente) de *tokenizar* (eso es, separar la expresión en *tokens* o trozos reconocibles), de clasificarlos.

# Árbol sintáctico (AST)
- AST (*Abstract syntax tree*) es una representación abstracta de la estructura de una expresión matemática (cumple el papel de código fuente). Es el núcleo de la librería (al menos de la parte que se encarga de las expresiones matemáticas literales).
- `Matcal`.`AstNode` guarda los datos necesarios para mantener una *árbol* (me refiero a la estructura de datos en general)
  - `Matcal`.`AstNode`.`Data` almacena los datos asociados al nodo
  - `Matcal`.`AstNode`.`Next` es una referencia al siguiente hermano del nodo
  - `Matcal`.`AstNode`.`Prev` es una referencia al anterior hermano del nodo
  - `Matcal`.`AstNode`.`Parent` es una referencia al padre del nodo
  - `Matcal`.`AstNode`.`Children` es una referencia al primer hijo del nodo
    > Nótese aquí que `Matcal`.`AstNode` son dos listas doblemente enlazadas en una: una lista que representa los hermanos (`Next` y `Prev`) y otra que representa los padres e hijos (`Parent` y `Children`)
- `Matcal`.`AstData` guarda los datos asociados a un *token* específico
  - `Matcal`.`AstData`.`Symbol` es el *token* tal cual aparece en la expresión (ie: "*sin*" es un *token* de la expresión "**8 * sin (x + 1)**")
  - `Matcal`.`AstData`.`Type` es el tipo del *token* tal cual fue clasificado por las reglas usadas en el análisis de la expresión (abstraído claro) (ie: en el ejemplo anterior, "*sin*" sería de tipo `Matcal`.`AstSymbolType`.`Function` al igual que "*+*" o "*\**", en cambio, "*8*" sería de tipo `Matcal`.`AstSymbolType`.`Constant`)

# Objetos internos
- `Matcal`.`Object` es el núcleo alrededor del cual se construyen todos los tipos de la máquina virtual implementada en `Matcal`.`Core` (es una clase abstracta de la que se derivan las tipos representables en la stack). Internamente representa una lista doblemente enlazada de objetos `Matcal`.`Object`. La stack en sí se sirve de esta representación para construirse.
  - `Matcal`.`Nil` representa un valor nulo
  - `Matcal`.`Closure` representa una función de primera clase que puede ser invocada usando los valores en la stack
  - `Matcal`.`Number` representa una valor numérico; básicamente la librería esta construida alrededor de este tipo. Almacena valores reales (enteros, fraccionarios y reales a su tiempo) de precisión infinita, eso es, el número puede tener infinitos lugares decimales, o representar un entero lo grande que necesite, sin perder precisión. Para minimizar el espacio requerido para almacenar una valor, este objeto guarda los números como enteros, fraccionarios o reales según se necesite, y transforma su valor para acomodarse a las operaciones matemáticas en las que se utilizan

# Análisis paso a paso
- Valida la entrada (debe estar codificada en UTF-8)
- Rompe la entrada en *tokens* reconocibles (*tokenizar*)
  - Escanea la string de entrada usando expresiones regulares buscando *tokens*: usando una lista de expresiones regulares que reconocen *tokens* específicos, compara cada entrado con cada expresión regular de forma sucesiva. Las entradas (mejor dicho, las partes de ella) que sean reconocibles por estas se agregan al vector de salida, mientras que las que no se reconozcan se siguen procesando. Al final, toda la entrada obtiene una coincidencia con alguna expresión regular (está garantizado porque la ultima expresión reconoce cualquier entrada); el vector resultante consta de los *tokens* en el orden en que aparacen en la entrada
- Usando el algoritmo **Shunting-yard** construye el árbol sintáctico

# Compilador
- El compilador usa [DynASM](http://luajit.org/dynasm.html), un generador de JITs, para construir un pequeño programa (o no, depende de la expresión) en código nativo de la plataforma en la que se ejecuta (por ahora solo esta soportada la arquitectura **x86_64** para ambos *Windows* y *Unix-like*) cuando la expresión se invoca a traves de la API de `Matcal`.`Core`
  - Recorre el árbol para anotar las constantes, funciones y variables que están presentes. Las constantes y funciones se agregan al código generado
    ```c
      /* ejemplo del bloque de datos  */
      /* para la entrada `4 + 5`      */
    data1:
      .asciz "+"
    data2:
      .asciz "4"
    data3:
      .asciz "5"
     ```
  - Recorre nuevamente el árbol, pero esta vez genera el código que ejecuta la expresión. Las variables se toman por orden de aparición (en **8 * y + x** la función quedaría **f(y,x)**); las funciones se toman del registro global (en el momento que el código se ejecuta) y las constantes de la cadena literal almacenada anteriormente.
    ```c
      /* ejemplo del bloque de código   */
      /* para la entrada `f(x) = 4 + x` */
      /* arquitectura: x86_64           */
      /* sistema operativo: linux       */
      /* convenio de llamadas: fastcall */
    main:
      sub rsp, 8      // Prepara la stack para una variable local
      mov [rsp], rdi  // Copia el argumento `core` en la stack

      mov rdi, [rsp]  // Agrega a la stack de `core` la función '+'
      lea rsi, data1  // Esto es una etiqueta en el bloque de datos
      call Matcal.Core.GetGlobal

      mov rdi, [rsp]  // Agrega a la stack de `core` el número '5'
      lea rsi, data2  // Esto es una etiqueta en el bloque de datos
      call Matcal.Core.PushNumber

      mov rdi, [rsp]  // Agrega una copia del valor de 'x'
      lea rsi, 0      // El índices de la variable es 0
      call Matcal.Core.PushValue

      mov rdi, [rsp]
      mov rsi, 2      // Ejecuta la función '+' con dos argumentos
      mov rdx, 1      // Retorna solo un valor
      call Matcal.Core.Call

      add rsp, 8      // Restablece la stack
      mov rax, 1      // Retorna 1
      ret
    ```

# Librería estándar
Internamente `Matcal` no implementa ninguna operación matemática, por lo que una librería auxiliar, `Matlib` implementa dichas operaciones. Por ahora tiene los operadores: "+" (suma usual), "-" (sustracción usual), "*" (multiplicación usual) y "/" (división usual). Ademas implementa algunas funciones matemáticas comunes
