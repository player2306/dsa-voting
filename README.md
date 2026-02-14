# Moore's Voting Algorithm
Data Structures and Algorithms | ChatGPT

**Goal:** Explain the algorithm, evaluate chatbot responses, and show benchmark results.

---

## Agenda
- Introduction
- Chatbot interactions
- Evaluation
- Application demo
- Benchmark results
- Reflection and improvements

---

## Introduction
- Moore's Voting Algorithm finds a majority element in a list.
- Chosen because Majority Elements was covered in class.
- Highlights how counters reduce time and space.
- ChatGPT helped clarify concepts, provide examples, and organize the discussion.

---

## Interaction 1 - Definition
**Q:** What is Moore's Voting Algorithm and what problem does it solve?

**A:** Linear-time, constant-space method to find a majority element (> n/2) by canceling out different elements.

**Note:** Accurate and concise.

---

## Interaction 2 - Step-by-Step
**Q:** Can you explain how Moore's Voting Algorithm works step by step?

**A:** Example: [2, 2, 1, 1, 2]

```
candidate = null, count = 0
see 2 -> candidate = 2, count = 1
see 2 -> count = 2
see 1 -> count = 1
see 1 -> count = 0
see 2 -> candidate = 2, count = 1
```

**Note:** Clear illustration of cancellation.

---

## Interaction 3 - Complexity
**Q:** What is the time and space complexity and why is it optimal?

**A:** Time O(n), Space O(1); any solution must inspect all elements.

**Note:** Matches theory and our implementation.

---

## Interaction 4 - Applications
**Q:** What are real-world applications?

**A:** Live polls, streaming vote counts, sensor consensus, network analysis, game input smoothing.

**Note:** Relevant and practical.

---

## Interaction 5 - Pros, Cons, Variants
**Q:** Advantages and limitations?

**Pros:** O(n) time, O(1) space, single pass, streaming friendly.

**Cons:** Only strict majority, requires verification, no frequency counts.

**Q:** Extension to > n/3?

**A:** Track two candidates and verify.

---

## Interaction 6 - Edge Cases + Comparison
**Edge cases:** empty array, single element, no majority, all identical, alternating values.

**Comparison:** Moore's is O(n) and streaming; sorting is O(n log n) and needs all data.

---

## Evaluation Summary
- Accuracy: 10/10
- Completeness: 8.5/10
- Clarity: 9/10
- Relevance: 10/10
- Overall: 9/10

**Key point:** Responses were correct and focused; minor gaps in verification emphasis.

---

## Application Example - Online Poll
- Votes arrive as a stream; storing all votes is impractical.
- Maintain candidate + count in one pass.
- Verification pass confirms majority at the end.
- Fits well due to O(n) time and O(1) memory.

---

## Benchmark Results (Graph)
- Growing array size up to 1,000,000.
- Moore's Voting: roughly linear growth, ~0.03 us at 1 to ~709 us at 1,000,000.
- Sort-based: grows as O(n log n), ~28 ms at 1,000,000.
- Brute force: O(n^2), impractical after 50,000.

---

## Reflection
- Strength: Fast, clear explanations and example-driven learning.
- Weakness: Must verify the candidate when majority is not guaranteed.
- Prompting mattered: sharper questions improved the depth of answers.

---

## Improvements
- Emphasize mandatory verification step more strongly.
- Provide short pseudocode or a small code snippet for implementation clarity.
- Compare tradeoffs vs. hash maps or sorting in specific scenarios.

---

## Conclusion
- Moore's Voting is efficient for strict majority detection.
- ChatGPT was effective for learning and structuring the presentation.
- Benchmark results match expected complexity trends.
