package main

import (
	"fmt"
	"log"

	"xenapi"
)

func newSession() *xenapi.Session {
	session := xenapi.NewSession(&xenapi.ClientOpts{
		URL: fmt.Sprintf("https://%s", host),
	})
	if _, err := session.LoginWithPassword(user, pass, "1.0", "xapi-cli"); err != nil {
		log.Fatalf("Login failed: %v", err)
	}
	return session
}
