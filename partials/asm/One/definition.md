# Primer parcial

Almacene en la memoria RAM usando **direccionamiento indirecto** el primer nombre, luego lea los datos o bits sobrantes del puerto F, K e interprételos como un tiempo entre 0 y 3 segundos usando el dipswitch, el nombre almacenado debe mostrarse secuencialmente en el display 14 segmentos por un periood igual al tiempo leído (periodo 0 es dejar la letra quieta).

Adicionalmente por medio de una interrupción externa se conectará un pulsador que cuando tenga un flanco de bajada hará que la letra mostrada se vea durante los siguientes tres períodos de tiempo con el nombre.

Cada segmento del display esta conectado a los pines de forma que:

> **A:** F2
> **B:** F1
> **C:** F5
> **D:** F0
> **E:** F4
> **F:** F3
> **G:** F7

> **H:** K2
> **I:** K2
> **J:** K5
> **K:** K0
> **L:** K4
> **M:** K3
> **N:** K7
