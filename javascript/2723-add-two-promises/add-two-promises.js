async function addTwoPromises(promise1, promise2) {
    const [a, b] = await Promise.all([promise1, promise2]);
    
    return a + b;
}

/**
 * addTwoPromises(Promise.resolve(2), Promise.resolve(2))
 * .then(console.log); // 4
 */