#ifndef CONTACT_GENERATOR_H
#define CONTACT_GENERATOR_H

class Contact;

class ContactGenerator
{
public:
	virtual ~ContactGenerator() {}

	virtual unsigned AddContact(Contact *contact, unsigned limit) const = 0;
};

#endif