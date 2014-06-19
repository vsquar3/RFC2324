How this works
=======

So this weird crap works kinda like this,


You send a a "BREW" request to the arduino to get the machine to start brewing coffee. Not hard. Kinda like this,

```
BREW /1 COFFEE/1.1
User-Agent: curl/7.37.0
Host: 10.0.0.177

```

This will return a 600 response saying all is good and coffee is brewing.

```
COFFEE/1.0 600 OK
Content-Type: application/coffee-pot-response
Status: brewing

```

You can also issue a GET request to find out what the machine is doing.

```
GET /1 COFFEE/1.1
User-Agent: curl/7.37.0
Host: 10.0.0.177

```

This will return a reponse with the status of the pot.

```
COFFEE/1.0 600 OK     
Content-Type: application/coffee-pot-response    
Status: {brewing,warming,idle}

```

When you want the machine to stop making coffee, issue a WHEN request.

```
WHEN /1 COFFEE/1.1
User-Agent: curl/7.37.0
Host: 10.0.0.177

```

It will stop making coffee and should respond with its status as idle.

```
COFFEE/1.0 600 OK
Content-Type: application/coffee-pot-response
Status: idle

```

