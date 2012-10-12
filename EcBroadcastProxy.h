
/** Enterprise Control Configuration and Logging

   Copyright (C) 2012 Free Software Foundation, Inc.

   Written by: Nicola Pero <nicola.pero@meta-innovation.com>
   Date: Febrary 2010
   Originally developed from 1996 to 2012 by Brainstorm, and donated to
   the FSF.

   This file is part of the GNUstep project.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02111 USA.

   */

#ifndef _ECBROADCASTPROXY_H
#define _ECBROADCASTPROXY_H

#import <Foundation/NSObject.h>

@class	NSArray;
@class	NSMutableArray;
@class	NSString;

enum EcBroadcastProxyError
{
  BCP_NO_ERROR = 0,
  BCP_COULD_NOT_CONNECT = 1,
  BCP_CONNECTION_WENT_DOWN = 2,
  BCP_MESSAGING_TIMEOUT = 3
};

/**
 * An EcBroadcastProxy instance forwards messages to multiple servers
 * via distributed objects.  This is great if you want to send a task
 * to be repeated by multiple servers in order to make a system more
 * scalable.<br />
 * You may also design your servers so that you pass some sort of
 * identifier to let them know that  one of them (which recognises
 * the identifier) should do one thing while the others ignore it.<br />
 * Finally, remote servers are always listed in the same order,
 * so you can access them by their index; to get the host/name, you
 * can get it by asking the receiverNames and receiverHosts and then 
 * looking up the one at the index you want.  This allows you to send
 * specific messages to specific servers.
 */
@interface EcBroadcastProxy : NSObject
{
  /* The names of the receiver object */
  NSArray *receiverNames;
  /* The hosts the receiver objects are on */
  NSArray *receiverHosts;

  /* The [proxy to the] remote objects */
  NSMutableArray *receiverObjects;

  /* The delegate (if any) */
  id delegate;

  /* The statistical info about what we did */

  /* Messages returning void */
  int onewayFullySent; 
  int onewayPartiallySent;   
  int onewayFailed; 
  /* Messages returning id */
  int idFullySent; 
  int idPartiallySent;   
  int idFailed; 
}

- (id) initWithReceiverNames: (NSArray *)names
	       receiverHosts: (NSArray *)hosts;

/** Configuration array contains a list of dictionaries (one for each
 * receiver) - each dictionary has two keys: `Name' and `Host', with
 * the corresponding values set.
 */
- (id) initWithReceivers: (NSArray *)receivers;

/* Methods specific to EcBroadcastProxy (which should not be forwarded 
 * to remote servers) are prefixed with `BCP' to avoid name clashes.  
 * Anything not prefixed with BCP is forwarded. */

/** Create connections to the receivers if needed.  It is called
 * internally when a message to broadcast comes in; but you may want
 * to call this method in advance to raise the connections so that
 * when a message to broadcast comes in, the connections are already
 * up and ready. */
- (void) BCPraiseConnections;

/** Get a string describing the status of the broadcast object */
- (NSString *) BCPstatus;

/** Set a delegate.<br /> 
 * The delegate object gets the messages from the BCPdelegate informal protocol
 * upon connection loss and when a connection is made.
 */
- (void) BCPsetDelegate: (id)object;

- (id) BCPdelegate;

/* [Advanced stuff] 
   Access to a single one of the servers we are broadcasting to.

   Eg, after sending a search to multiple servers, you might need to
   get further information only from the servers which answered
   affirmatively to your query.  To do it, use BCPproxyForName:host:
   to get the single remote server(s) you want to talk to, and talk to
   it (each of them) directly. */

/* Get the list of servers */
- (NSArray *) BCPreceiverNames;

- (NSArray *) BCPreceiverHosts;

/* Get only the number of receivers */
- (int) BCPreceiverCount;

/* Raise connection to server at index */
- (void) BCPraiseConnection: (int)index;

/* The following one gives you back the proxy to talk to.  
   It automatically BCPraise[s]Connection to that server before sending 
   you back a proxy.  It returns nil upon failure. */
- (id) BCPproxy: (int)index;

@end

@interface NSObject (BCPdelegate)

- (void) BCP: (EcBroadcastProxy *)proxy
  lostConnectionToServer: (NSString *)name
  host: (NSString *)host;

- (void) BCP: (EcBroadcastProxy *)proxy
  madeConnectionToServer: (NSString *)name
  host: (NSString *)host;

@end

#endif /* _BROADCASTPROXY_H */




