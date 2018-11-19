/**
 * CpuInfo
 * Copyright (c) 2017-2018, Aidan Khoury. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file error.h
 * @author Aidan Khoury (ajkhoury)
 * @date 11/16/2018 
 */

#ifndef _ERROR_H_
#define _ERROR_H_

#ifndef STATUS_DEFINED
#define STATUS_DEFINED

typedef int status_t;
typedef status_t STATUS;
typedef STATUS *PSTATUS;

#ifndef SUCCESS
#define SUCCESS(Status)     ((Status) >= 0)
#endif

#endif // !STATUS_DEFINED

/** Max count of errors */
#define MAX_ERRNO            4095

/** Success codes */
#define R_OK                0
#define E_OK                0
#define E_SUCCESS           0
#define STATUS_OK           0

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS      0
#endif//!STATUS_SUCCESS

/** Warning Codes */
#define E_EMPTY             2

/** Generic error code */
#define E_ERROR             (-1)    //!< No additional error information

/** Basic, standard, catch-all error codes */
#define E_INVALID           (-2)    //!< General invalid error code
#define E_PERM              (-3)    //!< Permission denied
#define E_NOMEM             (-4)    //!< Memory allocation or freeing error
#define E_BUSY              (-5)    //!< Resource is in use
#define E_NOTFOUND          (-6)    //!< Generic things that don't exist
#define E_BADADDRESS        (-7)    //!< Invalid pointer/address
#define E_TIMEOUT           (-8)    //!< A timeout has occured during an operation
#define E_ACCESS            (-9)    //!< Access denied

/** Specific error codes */
#define E_NOTINITIALIZED    (-10)   //!< The resource hasn't been initialized
#define E_NULLPARAM         (-11)   //!< NULL pointer passsed as a parameter
#define E_NODATA            (-12)   //!< There's no data on which to operate
#define E_BADDATA           (-13)   //!< The data being operated on is corrupt
#define E_ALIGN             (-14)   //!< Memory alignment errors
#define E_NOFREE            (-15)   //!< No free (whatever is being requested)
#define E_DEADLOCK          (-16)   //!< The action would result in a deadlock
#define E_PARADOX           (-17)   //!< The requested action is paradoxical
#define E_NOLOCK            (-18)   //!< The requested resource could not be locked
#define E_NOVIRTUAL         (-19)   //!< Virtual address space error
#define E_EXECUTE           (-20)   //!< Could not execute a command or program
#define E_NOTEMPTY          (-21)   //!< Attempt to remove something that has content
#define E_NOCREATE          (-22)   //!< Could not create an item
#define E_NODELETE          (-23)   //!< Could not delete an item
#define E_INTR              (-24)   //!< Interrupt error
#define E_IO                (-25)   //!< Input/Output error
#define E_BOUNDS            (-26)   //!< Array bounds exceeded, etc
#define E_ARGUMENTCOUNT     (-27)   //!< Incorrect number of arguments to a function
#define E_ALREADY           (-28)   //!< The action has already been performed
#define E_DIVIDEBYZERO      (-29)   //!< You're not allowed to do this!
#define E_DOMAIN            (-30)   //!< Argument is out of the domain of math func
#define E_RANGE             (-31)   //!< Result is out of the range of the math func
#define E_CANCELLED         (-32)   //!< Operation was explicitly cancelled
#define E_KILLED            (-33)   //!< Process or operation was unexpectedly killed
#define E_NOMEDIA           (-34)   //!< A removable disk has no media present.
#define E_NOTREADY          (-35)   //!< Device is not ready
#define E_NOSUCHENTRY       (-36)   //!< No such entry
#define E_NOSUCHDEVICE      (-37)   //!< No such device

/** File error codes */
#define E_NOSUCHFILE        (-38)   //!< No such file
#define E_NOSUCHDIR         (-39)   //!< No such directory
#define E_NOTFILE           (-40)   //!< The item is not a regular file
#define E_NOTDIR            (-41)   //!< The item is not a directory
#define E_NOWRITE           (-42)   //!< The item cannot be written

/** Not found error codes */
#define E_NOSUCHUSER        (-43)   //!< The used ID is unknown
#define E_NOSUCHPROCESS     (-44)   //!< The process in question does not exist
#define E_NOSUCHDRIVER      (-45)   //!< There is no driver to perform an action
#define E_NOSUCHFUNCTION    (-46)   //!< The requested function does not exist

/** Not implemented error code */
#define E_UNIMPLEMENTED     (-47)   //!< Functionality that hasn't been implemented

/** Buffer Overflow */
#define E_OVERFLOW          (-48)   //!< Buffer overflow.

/** Not Supported */
#define E_UNSUPPORTED       (-49)   //!< Not supported.

/** Queue Full */
#define E_QUEUEFULL         (-50)   //!< Queue is full.

/** CPUID Error */
#define E_CPUID             (-51)   //!< CPUID failed.

/** Feature Not Available Error */
#define E_FEATURE           (-52)   //!< Feature not available

#endif // _ERROR_H_
