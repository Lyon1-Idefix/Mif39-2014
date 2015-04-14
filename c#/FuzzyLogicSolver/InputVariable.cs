using System;
using System.Collections.Generic;

namespace FuzzyLogicSolver
{
	public class InputVariable : Variable
	{
		public InputVariable ()
		{
		}
		//
		public FuzzyLogic.InputVariable theInput { get; set; }
	}
	public class InputList : List < InputVariable > {}
}

