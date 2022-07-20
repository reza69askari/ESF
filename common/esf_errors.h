/*
 * esf_errors.h
 *
 * @Created: 6/30/2022 22:01:43
 *  @Author: Mohammadreza Askari reza69.askari@gmail.com
 */

#ifndef ESF_ERRORS_H_
#define ESF_ERRORS_H_

//! >>>>>>> exist files
#include <conf_board.h>

#if ((defined DEBUG) && (!defined Conf_AvoidExceptions))
#	define _ASSERT_ENABLE_
//#ifdef Conf_CatchAllExceptions //TODO: #ToDo: unit_test/suite.h !!?? is ASF ??
//	#define TEST_SUITE_DEFINE_ASSERT_MACRO
//#endif
#endif

/* >True as error (condition != 0)
 * 
 * if NOT defined Conf_Enable_HangOnExceptions on DEBUG mode (or _ASSERT_ENABLE_)
 * Hang on true condition (or run exception handler if Conf_CatchAllExceptions is defined)
 **/
#define Exception(e)				Assert(!(e))

 //! >>>>>>> Copy of C# exceptions (value changed for internal usage!)

typedef enum __attribute__((packed))
{
	__SystemExceptions		=0xF100,/* System Exceptions: */
	AccessViolationException	,	/* It is thrown when try to read or write protected memory. */
	AggregateException			,	/* Represents one or more errors that occur during application execution. */
	AppDomainUnloadedException	,	/* It is thrown when try to access an unloaded application domain. */
	ApplicationException		,	/* It is base class for application-defined exceptions. */
	ArgumentException			,	/* It is thrown when invalid argument provided to a method. */
	ArgumentNullException		,	/* It is thrown when a method requires argument but no argument is provided. */
	ArgumentOutOfRangeException	,	/* It is thrown when value of an argument is outside the allowable range. */
	ArithmeticException			,	/* It is thrown when doing arithmetic, casting, or conversion operation. */
	ArrayTypeMismatchException	,	/* It is thrown when try to store an element of the wrong type within an array. */
	BadImageFormatException		,	/* It is thrown when file image, dll or exe program is invalid. */
	CannotUnloadAppDomainException,	/* It is thrown when try to unload an application domain fails. */
	ContextMarshalException		,	/* The exception that is thrown when an attempt to marshal an object across a context boundary fails. */
	DataMisalignedException		,	/* It is thrown thrown when a unit of data is read from or written to an address that is not a multiple of the data size. */
	DivideByZeroException		,	/* It is thrown when there is an attempt to divide an integral or decimal value by zero. */
	DllNotFoundException		,	/* It is thrown when a DLL specified in a DLL import cannot be found. */
	DuplicateWaitObjectException,	/* The exception that is thrown when an object appears more than once in an array of synchronization objects. */
	EntryPointNotFoundException	,	/* The exception that is thrown when an attempt to load a class fails due to the absence of an entry method. */
	ExecutionEngineException	,	/* The exception that is thrown when there is an internal error in the execution engine of the common language runtime. */
	FieldAccessException		,	/* It is thrown when there is an invalid attempt to access a private or protected field inside a class. */
	FormatException				,	/* The exception that is thrown when the format of an argument is invalid, or when a composite format string is not well formed. */
	IndexOutOfRangeException	,	/* The exception that is thrown when an attempt is made to access an element of an array or collection with an index that is outside its bounds. */
	InsufficientMemoryException	,	/* The exception that is thrown when a check for sufficient available memory fails. This class cannot be inherited. */
	InvalidCastException		,	/* The exception that is thrown for invalid casting or explicit conversion. */
	InvalidOperationException	,	/* The exception that is thrown when a method call is invalid for the object's current state. */
	InvalidProgramException		,	/* The exception that is thrown when a program contains invalid Microsoft intermediate language (MSIL) or metadata. */
	InvalidTimeZoneException	,	/* The exception that is thrown when time zone information is invalid. */
	MemberAccessException		,	/* The exception that is thrown when an attempt to access a class member fails. */
	MethodAccessException		,	/* The exception that is thrown when there is an invalid attempt to access a method, such as accessing a private method from partially trusted code. */
	MissingFieldException		,	/* The exception that is thrown when there is an attempt to dynamically access a field that does not exist. */
	MissingMemberException		,	/* The exception that is thrown when there is an attempt to dynamically access a class member that does not exist. */
	MissingMethodException		,	/* The exception that is thrown when there is an attempt to dynamically access a method that does not exist. */
	MulticastNotSupportedException,	/* The exception that is thrown when there is an attempt to combine two delegates based on the Delegate type instead of the MulticastDelegate type. */
	NotCancelableException		,	/* It is thrown when an attempt is made to cancel an operation that is not cancelable. */
	NotFiniteNumberException	,	/* The exception that is thrown when a floating-point value is positive infinity, negative infinity, or Not-a-Number (NaN). */
	NotImplementedException		,	/* The exception that is thrown when a requested method or operation is not implemented. */
	NotSupportedException		,	/* The exception that is thrown when an invoked method is not supported, or when there is an attempt to read, seek, or write to a stream that does not support the invoked functionality. */
	NullReferenceException		,	/* The exception that is thrown when there is an attempt to dereference a null object reference. */
	ObjectDisposedException		,	/* The exception that is thrown when an operation is performed on a disposed object. */
	OperationCanceledException	,	/* The exception that is thrown in a thread upon cancellation of an operation that the thread was executing. */
	OutOfMemoryException		,	/* The exception that is thrown when there is not enough memory to continue the execution of a program. */
	OverflowException			,	/* The exception that is thrown when an arithmetic, casting, or conversion operation in a checked context results in an overflow. */
	PlatformNotSupportedException,	/* The exception that is thrown when a feature does not run on a particular platform. */
	RankException				,	/* The exception that is thrown when an array with the wrong number of dimensions is passed to a method. */
	StackOverflowException		,	/* The exception that is thrown when the execution stack overflows because it contains too many nested method calls. */
	SystemException				,	/* Serves as the base class for system exceptions namespace. */
	TimeoutException			,	/* The exception that is thrown when the time allotted for a process or operation has expired. */
	TimeZoneNotFoundException	,	/* The exception that is thrown when a time zone cannot be found. */
	TypeAccessException			,	/* The exception that is thrown when a method attempts to use a type that it does not have access to. */
	TypeInitializationException	,	/* The exception that is thrown as a wrapper around the exception thrown by the class initializer. This class cannot be inherited. */
	TypeLoadException			,	/* The exception that is thrown when type-loading failures occur. */
	TypeUnloadedException		,	/* The exception that is thrown when there is an attempt to access an unloaded class. */
	UnauthorizedAccessException	,	/* The exception that is thrown when the operating system denies access because of an I/O error or a specific type of security error. */
	UriFormatException			,	/* The exception that is thrown when an invalid Uniform Resource Identifier (URI) is detected. */
	
	__SystemDataExceptions	=0xF200,/* System.Data Exceptions: */
	ConstraintException			,	/* Represents the exception that is thrown when attempting an action that violates a constraint. */
	DataException				,	/* Represents the exception that is thrown when attempting an action that violates a constraint. */
	DBConcurrencyException		,	/* Gets or sets the value of the DataRow that generated the DBConcurrencyException. */
	DeleteRowInaccessibleException,	/* Represents the exception that is thrown when an action is tried on a DataRow that has been deleted. */
	DuplicateNameException		,	/* Represents the exception that is thrown when a duplicate database object name is encountered during an add operation in a DataSet -related object. */
	EvaluateException			,	/* Represents the exception that is thrown when the Expression property of a DataColumn cannot be evaluated. */
	InRowChangingEventException	,	/* Represents the exception that is thrown when you call the EndEdit method within the RowChanging event. */
	InvalidConstraintException	,	/* Represents the exception that is thrown when incorrectly trying to create or access a relation. */
	InvalidExpressionException	,	/* Represents the exception that is thrown when you try to add a DataColumn that contains an invalid Expression to a DataColumnCollection. */
	MissingPrimaryKeyException	,	/* Represents the exception that is thrown when you try to access a row in a table that has no primary key. */
	NoNullAllowedException		,	/* Represents the exception that is thrown when you try to insert a null value into a column where AllowDBNull is set tofalse. */
	OperationAbortedException	,	/* This exception is thrown when an ongoing operation is aborted by the user. */
	ReadOnlyException			,	/* Represents the exception that is thrown when you try to change the value of a read-only column. */
	RowNotInTableException		,	/* Represents the exception that is thrown when you try to perform an operation on a DataRow that is not in a DataTable. */
	StrongTypingException		,	/* The exception that is thrown by a strongly typed DataSet when the user accesses a DBNull value. */
	SyntaxErrorException		,	/* Represents the exception that is thrown when the Expression property of a DataColumn contains a syntax error. */
	TypedDataSetGeneratorException,	/* The exception that is thrown when a name conflict occurs while generating a strongly typed DataSet. */
	VersionNotFoundException	,	/* Represents the exception that is thrown when you try to return a version of a DataRow that has been deleted. */
	
	__SystemIOExceptions	=0xF300,/* System.IO Exceptions: */
	DirectoryNotFoundException	,	/* The exception that is thrown when part of a file or directory cannot be found. */
	DriveNotFoundException		,	/* The exception that is thrown when a drive that is referenced by an operation could not be found. */
	EndOfStreamException		,	/* An EndOfStreamException exception is thrown when there is an attempt to read past the end of a stream. */
	FileFormatException			,	/* The exception that is thrown when an input file or a data stream that is supposed to conform to a certain file format specification is malformed. */
	FileLoadException			,	/* The exception that is thrown when a managed assembly is found but cannot be loaded. */
	FileNotFoundException		,	/* The exception that is thrown when an attempt to access a file that does not exist on disk fails. */
	InternalBufferOverflowException,/* The exception thrown when the internal buffer overflows. */
	InvalidDataException		,	/* The exception that is thrown when a data stream is in an invalid format. */
	IOException					,	/* The exception that is thrown when an I/O error occurs. */
	PathTooLongException		,	/* The exception that is thrown when a path or file name is longer than the system-defined maximum length. */
	PipeException				,	/* Thrown when an error occurs within a named pipe. */
	
	__ExcsMAX	= 0xFFFF
} Exceptions_t;

#endif // ESF_ERRORS_H_