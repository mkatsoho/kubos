The Cubesat Space Protocol
==========================

Cubesat Space Protocol (CSP) is a small protocol stack written in C. CSP is designed to ease communication between distributed embedded systems in smaller networks, such as CubeSats. The design follows the TCP/IP model and includes a transport protocol, a routing protocol and several MAC-layer interfaces. The core of libcsp includes a router, a socket buffer pool and a connection oriented socket API.

The protocol is based on a 32-bit header containing both transport and network-layer information. Its implementation is designed for, but not limited to, embedded systems such as the 8-bit AVR microprocessor and the 32-bit ARM and AVR from Atmel. The implementation is written in GNU C and is currently ported to run on FreeRTOS or POSIX operating systems such as Linux.

The idea is to give sub-system developers of CubeSats the same features of a TCP/IP stack, but without adding the huge overhead of the IP header. The small footprint and simple implementation allows a small 8-bit system with less than 4 kB of RAM to be fully connected on the network. This allows all subsystems to provide their services on the same network level, without any master node required. Using a service oriented architecture has several advantages compared to the traditional mater/slave topology used on many CubeSats.

 * Standardised network protocol: All subsystems can communicate with eachother
 * Service loose coupling: Services maintain a relationship that minimizes dependencies between subsystems
 * Service abstraction: Beyond descriptions in the service contract, services hide logic from the outside world
 * Service reusability: Logic is divided into services with the intention of promoting reuse.
 * Service autonomy: Services have control over the logic they encapsulate.
 * Service Redundancy: Easily add redundant services to the bus
 * Reduces single point of failure: The complexity is moved from a single master node to several well defines services on the network

The implementation of LibCSP is written with simplicity in mind, but it's compile time configuration allows it to have some rather advanced features as well:

Features
--------

 * Thread safe Socket API
 * Router task with Quality of Services
 * Connection-oriented operation (RFC 908 and 1151).
 * Connection-less operation (similar to UDP)
 * ICMP-like requests such as ping and buffer status.
 * Loopback interface
 * Very Small Footprint 48 kB code and less that 1kB ram required on ARM 
 * Zero-copy buffer and queue system
 * Modular network interface system
 * Modular OS interface, ported to FreeRTOS, windows (cygwin) and Linux
 * Broadcast traffic
 * Promiscuous mode
 * Encrypted packets with XTEA in CTR mode
 * Truncated HMAC-SHA1 Authentication (RFC 2104)

LGPL Software license
---------------------
The source code is available under an LGPL 2.1 license. See COPYING for the license text.