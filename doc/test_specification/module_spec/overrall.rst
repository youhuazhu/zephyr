.. _module_overrall:

Overrall
#########

This is a software module test specification for zephyr RTOS prject. This document provides a generic verification specification suitable for use in IEC 61508 compliant developments.

**Software Integation Test Specification**::

    Version:              0.2
    Status:               Draft version
    Date:                 June 24th,  2020
    Document Author:
        Youhua Zhu        Intel IAGS SSP
    Document Reviewer(s):
        Anas Nashif       Software Architect, Intel IAGS SSP
        Jocelyn Li        Software Engineering Manager, Intel IAGS SSP
        Steven L Wang     V&V Engineer, Intel IAGS SSP
        Sartori Giovanni  Functional Safety Manager, Intel IAGS SSP
        Amber M Hibberd   Project Manager, Intel IAGS SSP
        Flavio Ceolin     Functional Safety Engineer, Intel IAGS SSP
    Document Approver(s):
        Amy R Occhialino  Engineering Manager, Intel IAGS SSP
        Anas Nashif       Software Architect, Intel IAGS SSP
        Amber M Hibberd   Project Manager, Intel IAGS SSP
        Sartori Giovanni  Functional Safety Manager, Intel IAGS SSP
    Applicable Standards: IEC 61508
    Template Version:     General_Verification_Specification.docx v1.0

            Intel Confidential

**Purpose**

    This document provides a generic verification specification suitable for use in IEC 61508 compliant developments.

**Audience**

    The audience for this document is internal developers of work products which require verification as well as teams carrying out verification activities.


**Acronyms and Terminology**

+------------+----------------------------------------------+
|  Term      | Description                                  |
+============+==============================================+
|  RTOS      | real time operate system                     |
+------------+----------------------------------------------+
|  Fusa      | Functional Safety                            |
+------------+----------------------------------------------+
|  IEC       | International Electrotechnical Commission    |
+------------+----------------------------------------------+


**Revision History**

+------------+------------+------------+--------------------------+
| Data       | Revision   | Auther     | Description              |
+============+============+============+==========================+
| 2020/06/24 | 0.1        |            | Initial draft version.   |
+------------+------------+------------+--------------------------+
| 2020/06/29 | 0.2        |            |- add content of overall. |
|            |            |            |- add more testcases.     |
+------------+------------+------------+--------------------------+
|            |            |            |                          |
+------------+------------+------------+--------------------------+

**Applicable Standards**

+----------------------------------+-----------------------------------------+
| Document                         | Document Number / Location              |
+==================================+=========================================+
|IEC 61508-2:2010, Functional      |                                         |
|safety of electrical/electronic/  |                                         |
|programmable electronic           |                                         |
|safety-related-systems – Part 2:  |         www.iec.ch                      |
|Requirements for electrical/      |                                         |
|electronic/programmable electronic|                                         |
|safety-related systems	           |                                         |
+----------------------------------+-----------------------------------------+
|IEC 61508-3:2010,Functional safety|                                         |
|of electrical/electronic/         |                                         |
|programmable electronic           |                                         |
|safety-related-systems – Part 3:  |         www.iec.ch                      |
|Software requirements             |                                         |
+----------------------------------+-----------------------------------------+

**Reference Documents**:

 - Zephyr kernel code.
 - Zephyr RTOS architecture specification and design specification
 - Verification and Validation Plan
 - Software Integration Test Plan
 - Zephyr official doc: https://docs.zephyrproject.org/


**License**

    No license (express or implied, by estoppel or otherwise) to any intellectual property rights is granted by this document.

    All information provided here is subject to change without notice. Contact your Intel representative to obtain the latest Intel product specifications and roadmaps. The products described may contain design defects or errors known as errata which may cause the product to deviate from published specifications. Current characterized errata are available on request.

    Copies of documents which have an order number and are referenced in this document may be obtained by calling 1-800-548-4725 or by
    visiting:  http://www.intel.com/design/literature.htm

    Intel technologies' features and benefits depend on system configuration and may require enabled hardware, software or service activation.
    Performance varies depending on system configuration. No computer system can be absolutely secure. Check with your system manufacturer or
    retailer or learn more at [intel.com].

    This material may relate to the creation of end products used in safety-critical applications designed to comply with functional safety
    standards or requirements (“Safety-Critical Applications”).  You agree and represent that you have all the necessary expertise to design,
    manage and ensure effective system-level safeguards to anticipate, monitor and control system failures in safety-critical applications.
    It is your sole responsibility to design, manage and assure system-level safeguards to anticipate, monitor and control system failures,
    and you agree that you are solely responsible for all applicable regulatory standards and safety-related requirements concerning your use
    of any material related to Safety Critical Applications.  You agree to indemnify and hold Intel and its representatives harmless against
    any damages, costs, and expenses arising in any way out of your use of the material related to Safety-Critical Applications.

    Intel, Intel brands, and the Intel logo are trademarks of Intel Corporation in the U.S. and/or other countries.

    Other names and brands may be claimed as the property of others.

    Copyright © 2020, Intel Corporation. All rights reserved.
