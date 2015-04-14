using System;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;
using Defuzzyfier = FuzzyLogicSolver.Defuzzyfier;

namespace FuzzyLogicSolver
{
	public class Proposition : Expression
	{
		public Proposition ()
		{
		}
		public FuzzyLogic.Proposition theProposition { get; set; }
		public Variable Variable { get; set; }
		public Curve Term { get; set; }
	}
}

