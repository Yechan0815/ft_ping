# ft_ping

Sends ICMP packets for echo to network host.

<br/>

# Header

IP HEADER (DEFAULT 20 byte)
```ts
0        1        2        3        4        5        6        7        8
+--------+--------+--------+--------+--------+--------+--------+--------+
|  Ver   |   IHL  |  TypeOfService  |            Total Length           |
+--------+--------+--------+--------+--------+--------+--------+--------+
|           Identification          |          Fragment Offset          |
+--------+--------+--------+--------+--------+--------+--------+--------+
|       TTL       |     Protocol    |              Checksum             |
+--------+--------+--------+--------+--------+--------+--------+--------+
|                             Source Address                            |
+--------+--------+--------+--------+--------+--------+--------+--------+
|                           Destination Address                         |
+--------+--------+--------+--------+--------+--------+--------+--------+
```

<br/>

ICMP HEADER (DEFAULT 8 byte)
```ts
0        1        2        3        4        5        6        7        8
+--------+--------+--------+--------+--------+--------+--------+--------+
|       Type      |       Code      |              Checksum             |
+--------+--------+--------+--------+--------+--------+--------+--------+
|             Identifier            |              Sequence             |
+--------+--------+--------+--------+--------+--------+--------+--------+
```
<br/>

PAYLOAD (DEFAULT 56 byte)
```ts
0        1        2        3        4        5        6        7        8
+--------+--------+--------+--------+--------+--------+--------+--------+
|                                .......                                |
+--------+--------+--------+--------+--------+--------+--------+--------+
|                                .......                                |

...
```

<br/>

# Compilation

compile the binary
```
make
```

remove object files & clean all
```
make clean
make fclean
```

recompile all sources
```
make re
```

<br/>

# Usage

**must be run with `sudo` to bind the socket**

![ubuntu-ping](https://user-images.githubusercontent.com/12230655/178921601-f833dd7f-fbe7-4060-81ae-cfa5b47ce97a.png)
