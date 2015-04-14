using System;
using System.Collections.Generic;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;
using Defuzzyfier = FuzzyLogicSolver.Defuzzyfier;

namespace FuzzyLogicSolver
{
	public class RuleBlock : Object
	{
		public RuleBlock ()
		{
		}
		public void Configure ( Conjunction conjunction, Disjunction disjunction, Activation activation )
		{
			Conjunction = conjunction;
			Disjunction = disjunction;
			Activation = activation;
		}
		public void Activate ()
		{
			foreach (Rule rule in Conclusions) {
				float activationDegree = rule.ActivationDegree (Conjunction,Disjunction);
				//Console.WriteLine ("Rule: {0} = {1}", rule, activationDegree);
				if (activationDegree > 0) {
					rule.Activate (activationDegree, Activation);
				}
			}
		}
		public FuzzyLogic.RuleBlock theBlock { get; set; }
		public RuleList Conclusions = new RuleList ();
		public Conjunction Conjunction { get; set; }
		public Disjunction Disjunction { get; set; }
		public Activation Activation { get; set; }
	}
	public class RuleBlockList : List < RuleBlock > {}
}

