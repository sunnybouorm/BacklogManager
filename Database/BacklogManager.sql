CREATE TABLE Activity
(
	ActivityID INT NOT NULL PRIMARY KEY,
	Name VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE Listing
(
	LID INT NOT NULL PRIMARY KEY,
	Title VARCHAR(255) NOT NULL,
	ActivityID VARCHAR(255) NOT NULL,
	FOREIGN KEY(ActivityID) REFERENCES Activity(ActivityID)
			ON UPDATE CASCADE
			ON DELETE CASCADE,
	CONSTRAINT uc_Title UNIQUE (ActivityID,Title)
);

CREATE TABLE UserDefinedField
(
	UDFID INT NOT NULL PRIMARY KEY,
	Name VARCHAR(255) NOT NULL,
	DataType VARCHAR(255) NOT NULL,
	Description VARCHAR(255),
	ActivityID VARCHAR(255) NOT NULL,
	FOREIGN KEY(ActivityID) REFERENCES Activity(ActivityID)
			ON UPDATE CASCADE
			ON DELETE CASCADE,
	CONSTRAINT uc_Name UNIQUE (Name, ActivityID)
);

CREATE TABLE UDFentry
(
	Data BLOB,
	UDFID VARCHAR(255) NOT NULL,
	FOREIGN KEY(UDFID) REFERENCES UserDefinedField(UDFID)
			ON UPDATE CASCADE
			ON DELETE CASCADE
);

CREATE TABLE Listing_UDFentry
(
	LID INT NOT NULL,
	UDFID VARCHAR(255) NOT NULL,
	EntryData BLOB,
	FOREIGN KEY(LID) REFERENCES Listing(LID)
			ON UPDATE CASCADE
			ON DELETE CASCADE,
	FOREIGN KEY(UDFID) REFERENCES UserDefinedField(UDFID)
			ON UPDATE CASCADE
			ON DELETE CASCADE,
	FOREIGN KEY(EntryData) REFERENCES UDFentry(Data)
			ON UPDATE CASCADE
			ON DELETE CASCADE
);