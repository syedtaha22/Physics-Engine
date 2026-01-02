"""
Random stable N-body system generator.

This module generates random gravitational N-body systems and filters
them via a burn-in simulation using leapfrog integration. The resulting
system is guaranteed to remain bounded for a configurable number of
integration steps.

Features
--------
- Central massive body
- Randomized orbiting bodies
- Leapfrog (kick-drift-kick) integration
- Optional 2D or 3D coordinate space
- CSV output compatible with external simulators
"""

import csv
import math
import random
import argparse

from tqdm import tqdm

G = 6.67430e-11
SOFTENING = 1e9
R_MAX = 1e13


class StableSystemGenerator:
    """
    Generator for random but dynamically stable N-body systems.

    The generator repeatedly samples random initial conditions and
    advances them using a leapfrog integrator. Systems that eject
    bodies or become numerically unstable during the burn-in phase
    are discarded.

    Parameters
    ----------
    n_bodies : int
        Total number of bodies including the central mass.
    burn_steps : int
        Number of integration steps used to test stability.
    dt : float
        Time step used for integration.
    dim : int
        Spatial dimension (2 or 3).
    """

    def __init__(self, n_bodies, burn_steps, dt, dim):
        self.n_bodies = n_bodies
        self.burn_steps = burn_steps
        self.dt = dt
        self.dim = dim

    def _compute_accelerations(self, bodies):
        """
        Compute gravitational accelerations for all bodies.

        Parameters
        ----------
        bodies : list of dict
            Body dictionaries containing mass and position.

        Returns
        -------
        list of tuple
            Acceleration vectors for each body.
        """
        n = len(bodies)
        acc = [tuple(0.0 for _ in range(self.dim)) for _ in range(n)]

        for i in range(n):
            ai = [0.0] * self.dim
            pi = bodies[i]["pos"]

            for j in range(n):
                if i == j:
                    continue

                pj = bodies[j]["pos"]
                d = [pj[k] - pi[k] for k in range(self.dim)]
                r2 = sum(x * x for x in d) + SOFTENING
                inv_r3 = 1.0 / (r2 * math.sqrt(r2))

                for k in range(self.dim):
                    ai[k] += G * bodies[j]["m"] * d[k] * inv_r3

            acc[i] = tuple(ai)

        return acc

    def _leapfrog_step(self, bodies):
        """
        Advance the system by one timestep using leapfrog integration.

        Parameters
        ----------
        bodies : list of dict
            Bodies to integrate in-place.
        """
        acc = self._compute_accelerations(bodies)

        for b, a in zip(bodies, acc):
            for k in range(self.dim):
                b["vel"][k] += 0.5 * a[k] * self.dt

        for b in bodies:
            for k in range(self.dim):
                b["pos"][k] += b["vel"][k] * self.dt

        acc = self._compute_accelerations(bodies)

        for b, a in zip(bodies, acc):
            for k in range(self.dim):
                b["vel"][k] += 0.5 * a[k] * self.dt

    def _is_stable(self, bodies):
        """
        Check whether the system remains bounded.

        A system is considered unstable if any body exceeds the
        maximum allowed radius or becomes non-finite.

        Parameters
        ----------
        bodies : list of dict
            Bodies to test.

        Returns
        -------
        bool
            True if the system is stable, False otherwise.
        """
        for b in bodies:
            r = math.sqrt(sum(x * x for x in b["pos"]))
            if not math.isfinite(r) or r > R_MAX:
                return False
        return True

    def _random_initial_system(self):
        """
        Generate a random initial N-body configuration.

        Returns
        -------
        list of dict
            Newly generated bodies with randomized positions and velocities.
        """
        bodies = []

        bodies.append({
            "m": 2e30,
            "pos": [0.0] * self.dim,
            "vel": [0.0] * self.dim
        })

        for _ in range(self.n_bodies - 1):
            r = random.uniform(5e10, 5e12)

            if self.dim == 2:
                theta = random.uniform(0, 2 * math.pi)
                pos = [r * math.cos(theta), r * math.sin(theta)]

                v_circ = math.sqrt(G * bodies[0]["m"] / r)
                ecc = random.uniform(0.0, 0.2)
                v = v_circ * math.sqrt(1 + ecc)

                direction = random.choice([-1.0, 1.0])
                vel = [
                    direction * (-v * math.sin(theta)),
                    direction * ( v * math.cos(theta))
                ]
            else:
                theta = random.uniform(0, 2 * math.pi)
                phi = random.uniform(0, math.pi)

                pos = [
                    r * math.sin(phi) * math.cos(theta),
                    r * math.sin(phi) * math.sin(theta),
                    r * math.cos(phi)
                ]

                v_circ = math.sqrt(G * bodies[0]["m"] / r)
                vel = [
                    -v_circ * math.sin(theta),
                     v_circ * math.cos(theta),
                     random.uniform(-0.1, 0.1) * v_circ
                ]

            bodies.append({
                "m": random.uniform(1e22, 1e25),
                "pos": pos,
                "vel": vel
            })

        return bodies

    def generate(self):
        """
        Generate and return a dynamically stable N-body system.

        Returns
        -------
        list of dict
            Stable system of bodies.
        """
        attempts = 0

        while True:
            attempts += 1
            bodies = self._random_initial_system()

            stable = True
            for _ in tqdm(range(self.burn_steps), desc=f"Attempt {attempts}"):
                self._leapfrog_step(bodies)
                if not self._is_stable(bodies):
                    stable = False
                    break

            if stable:
                print(f"Stable system found after {attempts} attempt(s).")
                return bodies


def write_csv(bodies, filename, dim):
    """
    Write a system of bodies to a CSV file.

    Parameters
    ----------
    bodies : list of dict
        Bodies to write.
    filename : str
        Output CSV file path.
    dim : int
        Spatial dimension.
    """
    with open(filename, "w", newline="") as f:
        w = csv.writer(f)
        headers = ["mass"]
        headers += [f"distance{axis}" for axis in "XYZ"[:dim]]
        headers += [f"velocity{axis}" for axis in "XYZ"[:dim]]
        w.writerow(headers)

        for b in bodies:
            w.writerow([b["m"]] + b["pos"] + b["vel"])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Random stable N-body generator")
    parser.add_argument("-n", type=int, default=200, help="Total number of bodies in the simulation (default: 200, includes the central mass).")
    parser.add_argument("--burn-steps", type=int, default=3000, help="Number of integration steps used to test stability before accepting a system (default: 3000).")
    parser.add_argument("--dt", type=float, default=3000, help="Time step in seconds used by the leapfrog integrator (default: 3000).")
    parser.add_argument("--dim", type=int, choices=[2, 3], default=2, help="Spatial dimension: 2 for planar, 3 for 3D space (default: 2).")
    parser.add_argument("--output", type=str, default="stable_random_system.csv", help="Output CSV file path (default: stable_random_system.csv).")
    parser.add_argument("--seed", type=int, default=None, help="Random seed for reproducibility (default: None, uses system time).")

    args = parser.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    print("Configuration:")
    print(f"  Number of bodies: {args.n}")
    print(f"  Burn-in steps: {args.burn_steps}")
    print(f"  Time step (dt): {args.dt} seconds")
    print(f"  Dimension: {args.dim}D")
    print(f"  Output file: {args.output}")
    print(f"  Random seed: {args.seed if args.seed is not None else 'None (system time)'}")
    print("Starting generation...")

    generator = StableSystemGenerator(args.n, args.burn_steps, args.dt, args.dim)
    bodies = generator.generate()
    write_csv(bodies, args.output, args.dim)
    print(f"Stable system written to {args.output}.")
