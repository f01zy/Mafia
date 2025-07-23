import type { ValidationError } from "express-validator"

export class ApiError {
  public status
  public errors
  public message

  constructor(status: number, message: string, errors: ValidationError[] = []) {
    this.status = status
    this.errors = errors
    this.message = message
  }

  public static UnauthorizedError() {
    return new ApiError(401, "User isn't auth.")
  }

  public static NotFound() {
    return new ApiError(404, "Not found.")
  }

  public static BadRequest(message: string, errors: ValidationError[] = []) {
    return new ApiError(400, message, errors)
  }
}
