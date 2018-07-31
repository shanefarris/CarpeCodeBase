
#pragma once

enum ErrorCodes : int32
{
	// Common
	Ok = 0,
	ErrorPopulating = -1,               // the entity's "Populate" method failed.
	DbError = -2,                       // General DB error, usually unable to save a record.
	Duplicate = -3,                     // Duplicate data being sent.
	NotFound = -4,
	ClientUnknownError = -988,
	Banned = -989,
	Permission = -990,
	InvalidEmail = -991,
	Validation = -992,
	NullInput = -993,
	InvalidValue = -994,
	InvalidVersion = -995,
	InvalidGameId = -996,
	InvalidUserId = -997,               // Unable to lookup data based on that user id, or the user id passed to the server is <= 0.
	MethodNotSupported = -998,          // You are setting the method type in the entity that is not supported on the server.
	UnknownError = -999,                // General exception caught in a try catch statement.  The exception message is saved in the database.

	// Create Category
	InvalidAccountToken = -20,
	AccountTypeNotExist = -21,          // You are trying to search, or save account data of a type that is not supported in the current version.
	GamerTagNotExist = -22,
	UnableToCreateLogin = -23,          // There was a general DB error and was unable to save the reccord.
	UnableToSaveToken = -24,

	// Create Account
	AccountTypeAlreadyExists = -30,     // You are trying to create an account with the same user Id and account type.
	DuplicateAccountData = -31,         // Data in the accounts table (aux1, aux2 and account token) are duplicated and can't create an account because of that.

	// Create Buddy
	AlreadyBuddies = -40,

	// News Feed
	InvalidBody = -50,
	InvalidTitle = -51,
	InvalidType = -52,
	InvalidFormat = -53,

	// Create User
	UniqueName = -60,
	UniqueEmail = -61,
	WeakPassword = -62,
	GamerTagShort = -63,
	GamerTagInvalid = -64,
	ErrorCreatingAccount = -65,
	ErrorSavingDeviceId = -66,

	// Permissions
	InvalidCategory = -70,
	AccountNotCreated = -71,

	// IAP Verify
	TokenExists = -72,
	InvalidToken = -73,

	// Purchase
	InsufficientFunds = -75,
	UnableToFindCurrency = -76,

	// Clans
	ApartOfClan = -80,
	NotInvited = -81,
	UnableToAddClanUser = -82,
	ClanDoesNotExist = -83,
	ClanRequestSent = -84,
	AlreadyMemeberOfThisClan = -85,
	InvalidRequester = -86,
	ClanNotEnabled = -87,
	ClanNameAlreadyExist = -88,

	// Messages
	MessageTooLong = -90,
	MessageDoesNotExist = -91,
	OnlyAnonymousEmail = -92,
	NoEmailAddressFound = -93,

	// Sale Offers
	OfferAlreadyExists = -100,
};
