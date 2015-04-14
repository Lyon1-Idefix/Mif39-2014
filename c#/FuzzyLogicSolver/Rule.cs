using System;
using System.Collections.Generic;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;
using Defuzzyfier = FuzzyLogicSolver.Defuzzyfier;

namespace FuzzyLogicSolver
{
	public class Rule : Object
	{
		public Rule ()
		{
		}
		public float ActivationDegree (Conjunction conjunction, Disjunction disjunction)
		{
			return theRule.Weight * antecedent.ActivationDegree (conjunction,disjunction);
		}
		public void Activate (float degree, Activation activation)
		{
			consequent.Modify (degree,activation);
		}
		public FuzzyLogic.Rule theRule { get; set; }
		public Antecedent antecedent { get; set; }
		public Consequent consequent { get; set; }
	}
	//
	public class RuleList : List < Rule > {}
}

